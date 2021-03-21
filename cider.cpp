#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/*
    Vector of all possible game sets.
    More information on these sets can be found here: https://boardgamegeek.com/wiki/page/Apples_to_Apples_Series
*/
static const std::vector<std::string> POSSIBLE_SETS = {
    "Core Set",
    "Core Expansion 1",
    "Core Expansion 2",
    "Core Expansion 3",
    "Core Expansion 4",
    "Party Box",
    "Party Box Expansion 1",
    "Kids Set",
    "Junior Set"
};

/*
    Unordered Map of all red cards. Red cards contain nouns.
    Noun => Flavor Text
*/
static std::unordered_map<std::string, std::string> RED = {};
/*
    Unordered Map of all green cards. Green cards contain adjectives.
    Adjective => Synonyms
*/
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
    Parameters: A path to the file to read, an int representing which deck to populate (0: Red, 1: Green; Set to Red by default), and a vector of indices of accepted sets
    Returns: Success boolean
    Description: Read the provided file and populate the specified deck's map with its values
*/
bool populateDecks(std::string path, int color = 0, const std::vector<int>& acceptedSets = {}) {
    std::ifstream inStream(path);

    if(!inStream.good()) {
        inStream.close();
        return false;
    }

    std::string line = "";
    std::vector<std::string> parts;
    // If the current line is in an accept set
    bool valid;
    // While the the stream still has lines in the file to look at
    while(!inStream.eof()) {
        getline(inStream, line);
        // If the current line isn't blank...
        if(line != "") {
            // Get the parts of the current line: item and flavor text / synonyms
            parts = splitStringByDelimiter(line, '\t');
            // Check if the acceptedSets Vector is not empty
            if(!acceptedSets.empty()) {
                // Set validity flag to false whenever a new line is looked at
                valid = false;
                // if not, loop through every single provided index in acceptedSets
                for(int index : acceptedSets) {
                    // If an accepted set is found in the second part of the line
                    if(parts[1].find(POSSIBLE_SETS[index]) != std::string::npos) {
                        // If so, set validity to true and break
                        valid = true;
                        break;
                    }
                    /*
                        The above conditional will only check to see if any one of the accepted sets appears in the line.
                        If an accepted set and unaccepted set are both present, it will read that the accepted set is present, and set validity to true.
                    */
                }
            } else {
                // If acceptedSets is empty, then all sets are valid. Set validity bool to true
                valid = true;
            }

            // If the current line is of a valid set
            if(valid) {
                // Add them to the specified deck's map
                // if color == 0 then deck is red
                if(color == 0) {
                    RED.insert({ parts[0], parts[1] });
                // else it is 1, which means deck is green
                } else {
                    GREEN.insert({ parts[0], parts[1] });
                }
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
    populateDecks(argv[1]/*, 0, { 0, 1, 2, 3, 4, 5, 6 }*/);
    // Populate the green deck's map using the contents of the second text file passed
    populateDecks(argv[2], 1/*, { 0, 1, 2, 3, 4, 5, 6 }*/);

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
