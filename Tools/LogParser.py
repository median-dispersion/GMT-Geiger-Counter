#!/usr/bin/env python3

import argparse
import sys
import re
import json
import csv
from datetime import datetime, timedelta
from dateutil import parser
from pathlib import Path

# =================================================================================================
# Get launch arguments
# =================================================================================================
def getLaunchArguments():

    # Launch argument parser
    parser = argparse.ArgumentParser(description="A python script for parsing log files from a GMT Geiger Counter into CSV tables. (https://github.com/median-dispersion/GMT-Geiger-Counter)")
    
    # Add arguments
    parser.add_argument("--files",  type=str, required=True,  nargs="+",                                         help="A list of log file paths separated by spaces that will be parsed into CSV tables. Additional '.part' files should be added to this list to combine them into one output. The list will be sorted by filename.")
    parser.add_argument("--date",   type=str, required=False, default=datetime(1970, 1, 1, 0, 0, 0).isoformat(), help="Start date of the log file recording. If the log file already contains date information, this will be ignored.")
    parser.add_argument("--output", type=str, required=False,                                                    help="Path of the output directory.")
    
    # Parse arguments
    return parser.parse_args()

# =================================================================================================
# Print a log message
# =================================================================================================
def log(level = "DEBUG", message = "Invalid log message!"):

    # Get the current date and time in ISO form
    date = datetime.now().astimezone().isoformat()
    
    # Depending on the log level color in the level text
    match level:

        case "DEBUG":   level = f"\033[92m[{level}]\033[0m"
        case "INFO":    level = f"\033[96m[{level}]\033[0m"
        case "WARNING": level = f"\033[93m[{level}]\033[0m"
        case "ERROR":   level = f"\033[91m[{level}]\033[0m"
        case _:         level = f"\033[95m[UNKNOWN]\033[0m"

    # Print log message
    print(f"{date} {level} >> {message}")

# =================================================================================================
# Terminate script execution
# =================================================================================================
def terminate():

    # Print log message
    log("INFO", f"Exiting!")
    
    # Exit
    sys.exit()

# =================================================================================================
# Get the natural key
# =================================================================================================
def getNaturalKey(values):

    # Split into parts: text and numbers
    return [int(text) if text.isdigit() else text.lower() for text in re.split(r"(\d+)", values)]

# =================================================================================================
# Read data from the log files
# =================================================================================================
def readLogFiles(files):

    # Raw log file data
    data = ""

    # Try reading log files
    try:

        # For each file sorted in order
        for file in sorted(files, key=getNaturalKey):

            # Open the file
            with open(file, "r") as logFile:

                # Read the log file content into data
                data += logFile.read()

            # Print log message
            log("INFO", f"Loaded '{file}'")

        # Return the log file data
        return data

    # If an exception occurs
    except Exception as exception:

        # Print log message
        log("ERROR", f"Reading log file failed! ({exception})")
        
        # Exit
        terminate()

# =================================================================================================
# Get the output directory path
# =================================================================================================
def getOutputPath(path, file):

    # Try getting the output path
    try:

        # If an output path is provided
        if path is not None:

            # Create path element
            path = Path(path)

            # If path is a file raise an exception
            if path.is_file(): raise ValueError(f"Output path '{path}' is not a directory!")

        # If no output path was provided
        else:

            # Use the current path as the output directory
            path = Path("./")

            # Input log file path
            file = Path(file)

            # Add log file name without extension to the output path
            path = path / file.with_suffix("").stem

        # Create directory if it doesn't exists
        path.mkdir(parents=True, exist_ok=True)

        # Return the output path
        return path

    # If an exception occurs
    except Exception as exception:

        # Print error message
        log("ERROR", f"Failed to initialize the output directory! ({exception})")
        
        # Exit
        terminate()

# =================================================================================================
# Get log start date
# =================================================================================================
def getStartDate(date):

    # Try parsing the date string
    try:    

        # Parse date
        date = parser.parse(date)

    # If parsing the date fails use the unix epoch as the start date
    except: 
        
        # Log warning message
        log("WARNING", f"Could not parse date '{date}'. Using '1970-01-01T00:00:00' instead...")

        # Use unix epoch as starting date
        date = datetime(1970, 1, 1, 0, 0, 0)

    # Return the date
    return date

# =================================================================================================
# Parse raw log file data into JSON data
# =================================================================================================
def getJSONData(raw, date):

    # JSON data
    data = []

    # If the raw data ends with a trailing "," remove it
    if raw.endswith(","): raw = raw[:-1]

    # Split the raw data into parts delimited by "},{"
    parts = re.split(r"(?<=})\s*,\s*(?={)", raw)

    # For every part in the data
    for part in parts:

        # Try parsing that part as JSON
        try:

            # Parse raw part data as JSON
            message = json.loads(part)
            
            # Append parsed JSON to data
            data.append(message)

        # If the raw part data couldn't be parsed as JSON
        except Exception as exception:

            # Print a warning message
            log("WARNING", f"Could not parse '{part}' as JSON! ({exception})")

    # Return the JSON data
    return data

# =================================================================================================
# Get all log messages in the JSON data
# =================================================================================================
def getLogMessages(data):

    # All log messages
    messages = {

        "geigerCounter":     [],
        "cosmicRayDetector": [],
        "systemInfo":        [],
        "systemEvents":      []

    }

    # For every message in the data
    for message in data:

        # Depending on the message type add it to the specific message list
        match(message["type"]):

            case "geigerCounter":     messages["geigerCounter"].append(message)
            case "cosmicRayDetector": messages["cosmicRayDetector"].append(message)
            case "system":            messages["systemInfo"].append(message)
            case "event":             messages["systemEvents"].append(message)

    # Return log messages
    return messages

# =================================================================================================
# Get message data
# =================================================================================================
def getMessageData(messages, date):

    # Message data
    data = []

    # Entry index
    index = 0

    # For every message
    for message in messages:

        # Increase the entry index
        index += 1

        # Set the entry index
        entry = {"index": index}

        # If the message contains a date use it
        # Else use the start date offset by the message time in ISO form
        if "date" in message: entry["date"] = message["date"]
        else                : entry["date"] = (date + timedelta(milliseconds=message["time"])).isoformat()

        # Add message time to the entry
        entry["time"] = message["time"]

        # Add message data to the entry
        entry = entry | message["data"]

        # Append entry to data list
        data.append(entry)

    # Return message data
    return data

# =================================================================================================
# Write message data to a CSV file
# =================================================================================================
def writeMessageData(data, file, fieldnames):

    # Only write to file there is any data
    if (len(data) > 0):

        # Get data keys
        keys = list(fieldnames.keys())

        # Get CSV header
        header = [fieldnames[key] for key in keys]

        # Try writing to file
        try:

            # Open output file
            with open(file, "w") as output:

                # Initialize CSV writer
                writer = csv.writer(output)

                # Write CSV header
                writer.writerow(header)

                # For every entry in data
                for entry in data:

                    # Match the data to the data key
                    row = [entry.get(key, "") for key in keys]

                    # Write data row
                    writer.writerow(row)

            log("INFO", f"Created '{file}'!")

        # If an error occurs writing to the output file
        except Exception as exception:

            # Log error message
            log("ERROR", f"Writing to the output file '{file}' failed! ({exception})")

# =================================================================================================
# Main
# =================================================================================================
def main():

    # Get launch arguments
    arguments = getLaunchArguments()

    # Sort input files in order of file name
    arguments.files = sorted(arguments.files, key=getNaturalKey)

    # Read in raw data from the log files
    raw = readLogFiles(arguments.files)

    # Get the output directory path
    output = getOutputPath(arguments.output, arguments.files[0])

    # Get the log start date
    date = getStartDate(arguments.date)

    # Parse raw data as JSON
    data = getJSONData(raw, date)

    # Sort JSON data into message data
    messages = getLogMessages(data)

    # Get message data for specific message type
    geigerCounter     = getMessageData(messages["geigerCounter"],     date)
    cosmicRayDetector = getMessageData(messages["cosmicRayDetector"], date)
    systemInfo        = getMessageData(messages["systemInfo"],        date)
    systemEvents      = getMessageData(messages["systemEvents"],      date)

    # Write geiger counter data to a CSV file
    writeMessageData(
        # Data
        geigerCounter,

        # Output file
        f"{str(output)}/Geiger_Counter_{Path(arguments.files[0]).stem}.csv",

        # Output fieldname mapping
        {
            "index":                "Index",
            "date":                 "Date",
            "time":                 "System time [Milliseconds]",
            "counts":               "Total number of counts",
            "mainCounts":           "Main tube counts",
            "followerCounts":       "Follower tube counts",
            "countsPerMinute":      "Counts per minute",
            "microsievertsPerHour": "Microsieverts per hour",
            "tubes":                "Number of tubes",
            "tubeType":             "Tube type" 
        }
    )

    # Write cosmic ray detector data to a CSV file
    writeMessageData(
        # Data
        cosmicRayDetector,

        # Output file
        f"{str(output)}/Cosmic_Ray_Detector_{Path(arguments.files[0]).stem}.csv",

        # Output fieldname mapping
        {
            "index":             "Index",
            "date":              "Date",
            "time":              "System time [Milliseconds]",
            "coincidenceEvents": "Coincidence events",
            "eventsTotal":       "Total number of coincidence events",
            "eventsPerHour":     "Coincidence events per hour",
            "mainCounts":        "Main tube counts",
            "followerCounts":    "Follower tube counts"
        }
    )

    # Write system info data to a CSV file
    writeMessageData(
        # Data
        systemInfo,

        # Output file
        f"{str(output)}/System_Info_{Path(arguments.files[0]).stem}.csv",

        # Output fieldname mapping
        {
            "index":    "Index",
            "date":     "Date",
            "uptime":   "System uptime [Milliseconds]",
            "heapSize": "Total heap [Bytes]",
            "freeHeap": "Free heap [Bytes]",
            "minHeap":  "Smallest heap size since boot [Bytes]",
            "maxBlock": "Largest allocatable block [Bytes]",
            "firmware": "Firmware version",
        }
    )

    # Write system event data to a CSV file
    writeMessageData(
        # Data
        systemEvents,

        # Output file
        f"{str(output)}/System_Events_{Path(arguments.files[0]).stem}.csv",

        # Output fieldname mapping
        {
            "index":  "Index",
            "date":   "Date",
            "time":   "System time [Milliseconds]",
            "source": "Source",
            "action": "Action",
        }
    )

# Start the main function
if __name__ == "__main__": main()