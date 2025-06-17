from pathlib import Path
from bs4 import BeautifulSoup
import rcssmin
import rjsmin
import htmlmin

# =================================================================================================
# Inline and minify CSS stylesheets
# =================================================================================================
def inlineCSS(html, root):

    # For every CSS stylesheet tag in the HTML content
    for stylesheet in html.find_all("link", rel="stylesheet", href=True):

        # Create a new HTML style tag
        style = html.new_tag("style")

        # Open the CSS file defined in the HTML content
        with open(root / stylesheet["href"], "r", encoding="utf-8") as file:

            # Read and minify the content of the CSS file into the new style tag
            style.string = rcssmin.cssmin(file.read())

        # Replace the stylesheet definition with the style tag
        stylesheet.replace_with(style)

# =================================================================================================
# Inline and minify JavaScript files
# =================================================================================================
def inlineJavaScript(html, root):

    # For every JavaScript tag in the HTML content
    for javascript in html.find_all("script", src=True):

        # Create a new HTML script tag
        script = html.new_tag("script")

        # Open the JavaScript file defined in the HTML content
        with open(root / javascript["src"], "r", encoding="utf-8") as file:

            # Read and minify the content of the JavaScript file into the new script tag
            script.string = rjsmin.jsmin(file.read())

        # Replace the JavaScript definition with the script tag
        javascript.replace_with(script)

# =================================================================================================
# Build a web app
# =================================================================================================
def buildApp(path):

    # Get absolute path of the web app files
    root = Path(path).resolve()

    # Open the index.html file
    with open(root / "index.html", "r", encoding="utf-8") as file:

        # Read in the HTML with BeautifulSoup
        html = BeautifulSoup(file, "html.parser")

    # Inline the CSS and JavaScript files
    inlineCSS(html, root)
    inlineJavaScript(html, root)

    # Return the minified HTML content
    return htmlmin.minify(str(html), remove_empty_space=True).replace("\n", "")

# =================================================================================================
# Build all web apps
# =================================================================================================

with open("./geiger-counter.html", "w") as file: file.write(buildApp("./Geiger Counter"))