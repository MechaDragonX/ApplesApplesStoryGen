#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Unordered Map of all red cards. Red cards contain nouns.
// Noun => Flavor Text
static std::unordered_map<std::string, std::string> RED = {};
// Unordered Map of all green cards. Green cards contain adjectives.
// Adjective => Synonyms
static std::unordered_map<std::string, std::string> GREEN = {};

// Taken from BambooCutterLayover (https://github.com/MechaDragonX/BambooCutterLayover)
/*
    Parameters: Reference to an input string, and a character to split by 
    Returns: A vector<string> that contains all the parts of the string
    Description: Takes a string, splits it by a delimiter character, and return a vector with all the parts of the string
*/
std::vector<std::string> splitStringByDelimiter(std::string& input, char delimiter) {
    std::vector<std::string> result = {};
    size_t pos = 0;
    std::string token;
    // Loop until the end of the string
    while((pos = input.find(delimiter)) != std::string::npos) {
        // Make the token the current portion
        token = input.substr(0, pos);
        // Remove blank matches
        if(token != "") {
            result.push_back(token);
        }
        // Remove everything looked at so far
        input.erase(0, pos + 1);
    }
    // Append the remaing bits to the vector
    result.push_back(input);
    return result;
}
/*
    Parameters: A path to the file to read and an int representing which deck to populate (0: Red, 1: Green). Set to Red by default
    Returns: Success boolean
    Description: Read the provided file and populate the specified deck's map with its values
*/
bool populateDecks(std::string path, int color = 0) {
    std::ifstream inStream(path);

    if(!inStream.good()) {
        inStream.close();
        return false;
    }

    std::string line = "";
    std::vector<std::string> parts;
    // While the the stream still has lines in the file to look at
    while(!inStream.eof()) {
        getline(inStream, line);
        // If the current line isn't blank...
        if(line != "") {
            // Get the parts of the current line: item and flavor text / synonyms
            parts = splitStringByDelimiter(line, '\t');
            // Add them to the specified deck's map
            if(color == 0) {
                RED.insert({ parts[0], parts[1] });
            } else {
                GREEN.insert({ parts[0], parts[1] });
            }
        } else {
            // There's always a new line at the end of the file, so there's nothing to see if the current line is blank
            break;
        }
    }

    inStream.close();
    return true;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "ERROR: That's too few arguments! You only need one argument!" << std::endl;
// If the program is running on Windows (does not apply to Cygwin, MSYS2, WSL, etc.)
#ifdef WINNT
        // Give proper usage based on Command Prompt and Powershell
        std::cout << "Usage on Command Prompt: cider.exe <text file for red deck> <text file for green deck>" << std::endl;
        std::cout << "Usage on PowerShell: .\\cider.exe <text file for red deck> <text file for green deck>" << std::endl;
// Otherwise, the user will most likely be on Unix
#else
        // Give proper usage based on Unix
        std::cout << "Usage: ./cider.out <text file for red deck> <text file for green deck>" << std::endl;
#endif
        return 1;
    }
    if(argc > 3) {
        std::cout << "ERROR: That's too many arguments! You only need one argument!" << std::endl;
// If the program is running on Windows (does not apply to Cygwin, MSYS2, WSL, etc.)
#ifdef WINNT
        // Give proper usage based on Command Prompt and Powershell
        std::cout << "Usage on Command Prompt: cider.exe <text file for red deck> <text file for green deck>" << std::endl;
        std::cout << "Usage on PowerShell: .\\cider.exe <text file for red deck> <text file for green deck>" << std::endl;
// Otherwise, the user will most likely be on Unix
#else
        // Give proper usage based on Unix
        std::cout << "Usage: ./cider.out <text file for red deck> <text file for green deck>" << std::endl;
#endif
        return 1;
    }

    // Populate the red deck's map using the contents of the first text file passed
    populateDecks(argv[1]);
    // Populate the green deck's map using the contents of the second text file passed
    populateDecks(argv[2], 1);

    // Initialize RNG with current time as seed
    srand(time(nullptr));
    // Iterate to random key in Red deck
    auto iterator = RED.begin();
    std::advance(iterator, rand() % RED.size());
    // Print out that random key and value
    std::cout << iterator->first + ": " + iterator->second << std::endl;
    // Iterate to random key in Green deck
    iterator = GREEN.begin();
    std::advance(iterator, rand() % GREEN.size());
    // Print out that random key and value
    std::cout << iterator->first + ": " + iterator->second << std::endl;
}
