import argparse
import re
import json
import csv
from datetime import datetime, timedelta
from dateutil import parser
from pathlib import Path

# =================================================================================================
# Get the scripts launch arguments
# =================================================================================================
def getLaunchArguments(): 

    # Launch argument parser
    parser = argparse.ArgumentParser(description="A script for parsing GMT Geiger counter log files.")
    
    # Add arguments
    parser.add_argument("--file",   type=str, required=True,  help="Path to the main '.json' log file. Additional '.part' files will be added automatically.")
    parser.add_argument("--date",   type=str, required=False, help="Start date of the log file recording.")
    parser.add_argument("--output", type=str, required=False, help="Path to the output directory.")
    
    # Parse arguments
    return parser.parse_args()

# =================================================================================================
# Get log start date
# =================================================================================================
def getStartDate(date):

    # Try parsing the date string, if that fails use the unix epoch as the start date
    try:    date = parser.parse(date)
    except: date = datetime(1970, 1, 1, 0, 0, 0)

    # Return the date
    return date

# =================================================================================================
# Get the output directory path
# =================================================================================================
def getOutputDirectory(path):

    # If an output path is provided
    if path is not None:

        # Create path element
        path = Path(path)

        # If path is a file get the parent directory
        if path.is_file(): path = path.parent

    # If no output path was provided
    else:

        # Use the current path
        path = Path("./")

    # Create directory if it doesn't exists
    path.mkdir(parents=True, exist_ok=True)

    # Return the output path
    return str(path)

# =================================================================================================
# 
# =================================================================================================
def getNaturalKey(string):

    return [int(text) if text.isdigit() else text.lower() for text in re.split(r"(\d+)", string)]

# =================================================================================================
# Get all log files
# =================================================================================================
def getLogFiles(file):

    # Get the path of the log file
    file = Path(file)

    # List of log files
    files = []

    # For all log part files in the log file directory
    for part in file.parent.glob(f"{file.name}.part*"):

        # Add part file to the list of log files
        files.append(str(part))

    # Sort the list of log files
    files = sorted(files, key=getNaturalKey)

    # Insert the main log file to the list of log files
    files.insert(0, str(file))

    # Return the list of log files
    return files

# =================================================================================================
# Get log file JSON data
# =================================================================================================
def getJSONData(files):

    # JSON log data
    data = []

    # For every log file
    for file in files:

        # Open the log file
        with open(file, "r") as log:

            # Read the log file
            raw = log.read()

            # While there is still raw data to parse
            while len(raw):

                # Try parsing raw data as JSON
                try:

                    # Parse raw data as JSON
                    message, index = json.JSONDecoder().raw_decode(raw)

                    # Append parsed JSON to the list of data
                    data.append(message)

                    # Remove parsed JSON from the raw data
                    raw = raw[index:]

                # If parsing the raw data as JSON fails
                except: 
                    
                    # Remove one character from the raw data
                    raw = raw[1:]

    # Return the list of log data
    return data

# =================================================================================================
# Get all log messages in the log message data
# =================================================================================================
def getLogMessages(data):

    # All log messages
    messages = {

        "geigerCounter":     [],
        "cosmicRayDetector": [],
        "systemInfo":        [],
        "systemEvents":      []

    }

    # For every entry in the log data
    for entry in data:

        # Depending on the message type add it to the specific message list
        match(entry["type"]):

            case "geigerCounter":     messages["geigerCounter"].append(entry)
            case "cosmicRayDetector": messages["cosmicRayDetector"].append(entry)
            case "system":            messages["systemInfo"].append(entry)
            case "event":             messages["systemEvents"].append(entry)

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

        # If the message contains a date use that
        # Else add the message time to the start date and use the resulting ISO date string
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

    # Only do write if there is any data
    if (len(data) > 0):

        # Get data keys
        keys = list(fieldnames.keys())

        # Get CSV header
        header = [fieldnames[key] for key in keys]

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

# =================================================================================================
# Main
# =================================================================================================
def main():

    arguments = getLaunchArguments()                 # Get the launch arguments
    date      = getStartDate(arguments.date)         # Get the log start date
    output    = getOutputDirectory(arguments.output) # Get the output directory
    files     = getLogFiles(arguments.file)          # Get all log files
    data      = getJSONData(files)                   # Get log file JSON data
    messages  = getLogMessages(data)                 # Get log messages

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
        f"{output}/Geiger_Counter_{Path(arguments.file).stem}.csv",

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
        f"{output}/Cosmic_Ray_Detector_{Path(arguments.file).stem}.csv",

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
        f"{output}/System_Info_{Path(arguments.file).stem}.csv",

        # Output fieldname mapping
        {
            "index":    "Index",
            "date":     "Date",
            "upTime":   "System uptime [Milliseconds]",
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
        f"{output}/System_Events_{Path(arguments.file).stem}.csv",

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