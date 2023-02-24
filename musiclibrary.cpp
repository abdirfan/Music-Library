/// Name: Abdullah Irfan
/// Project 2 - Music Library
/// System: VS Studio on Windows 11
/// Description: This program loads all the music files that are given and then allows the user to use search command to get their desired results.Users have the 
/// have the ability to search by song, artist, and album names. This alllows utilization of some more commands and they are described in detail in the
/// help-command.txt. Every music data file is set up the same way, and our system has a series of commands to interact with the data files (load & export) 
/// as well as displaying information about the data once it is in the program. I have used maps and sets within struct to store the music files.

/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <string>
#include <set>
#include <map>

using namespace std;

// INSERTS GIVEN HELPERS CODE BELOW - DO NOT REMOVE
#include "helpers.cpp"
// INSERTS GIVEN HELPERS CODE ABOVE - DO NOT REMOVE

//
// NOTE TO STUDENTS:
// You may use the functions in the helpers file,
// but you may not modify the file or its contents in any way whatsoever.
//

//
// STUDENT CODE MAY GO BELOW THIS LINE
//


//A struct which keeps all the data of the music library
struct MusicLibrary {
    set<string> artist;

    map<string, string> albums;
    map<string, set<string>> songToAlbums;
    map<string, set<string>> artistToAlbums;

    map<string, string> songs;
    map<string, string> artists;

    //clears the data from all the maps and sets
    void clear() {
        albums.clear();
        songToAlbums.clear();
        artistToAlbums.clear();
        artist.clear();
    }

    //insert an ablum in the library given the name
    void insertAlbum(string& albumName) {
        string lowerName = albumName;
        tolower(lowerName);
        albums.insert({ lowerName, albumName });
    }

    //insert an song in the library given the name of the song and title of the album
    void insertSong(string& song, string& album) {
        string originalName = song;

        tolower(song);

        songs[song] = originalName;

        songToAlbums[song].insert(album);
    }

    //insert an artist in the library given the name of the artis and title of the album
    void insertArtist(string& artistName, string& album) {
        string originalName = artistName;

        artist.insert(artistName);
        tolower(artistName);

        artists[artistName] = originalName;

        artistToAlbums[artistName].insert(album);
    }

    //Return functions for stats
    int getTotalSongs() {
        return songToAlbums.size();
    }
    int getTotalRecords() {
        return albums.size();
    }
    int getUniqueArtist() {
        return artist.size();
    }

    //Function taking all the information in the databse and printing it accordingly
    void print() {
        cout << "Your Current Music Library Includes" << endl;
        cout << "===================================" << endl;

        for (auto currAlbum : albums) {
            cout << currAlbum.second << endl;
            for (auto currArtist : artistToAlbums) {
                if (currArtist.second.find(currAlbum.second) != currArtist.second.end()) {
                    cout << " " << artists[currArtist.first] << endl;
                }
            }
            for (auto currSong : songToAlbums) {
                if (currSong.second.find(currAlbum.second) != currSong.second.end()) {
                    cout << "   " << songs[currSong.first] << endl;
                }
            }
        }
    }

    //search for all albums and insert the original name
    set<string> searchAlbum(string& album) {
        set<string> res;
        for (auto x : albums) {
            if (x.first.find(album) != x.first.npos) {
                res.insert(x.second);
            }
        }
        return res;
    }

    //search for all the songs and make a set containing all albums in which the song keyword appears
    set<string> searchSong(string& song) {
        set<string> res;
        for (auto x : songToAlbums) {
            if (x.first.find(song) != x.first.npos) {
                for (auto curr : x.second) {
                    res.insert(curr);
                }
            }
        }
        return res;
    }

    //return all albums which have the given artist keyword
    set<string> searchArtist(string& artist) {
        set<string> res;
        for (auto x : artistToAlbums) {
            if (x.first.find(artist) != x.first.npos) {
                for (auto curr : x.second) {
                    res.insert(curr);
                }
            }
        }
        return res;
    }

    //takes a filename and puts all the current data in that file
    void exportCommand(string& filename) {

        if (filename == "") {
            filename = "musicdatabase.txt";
        }
        ofstream file(filename);

        for (auto currAlbum : albums) {
            file << currAlbum.second << endl;
            for (auto currArtist : artistToAlbums) {
                if (currArtist.second.find(currAlbum.second) != currArtist.second.end()) {
                    file << artists[currArtist.first] << endl;
                }
            }
            for (auto currSong : songToAlbums) {
                if (currSong.second.find(currAlbum.second) != currSong.second.end()) {
                    file << songs[currSong.first] << endl;
                }
            }
            file << currAlbum.second << endl;
        }

    }
};

//function to check if the string is only containing of digits (helper funciton)
bool isNumber(string& s) {
    for (int i = 0; i < s.size(); i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

//insert artists till we see song numbers
void insertArtists(string& title, fstream& musicFile, MusicLibrary& allMusic) {
    string line;
    getline(musicFile, line);
    string first;
    string remaining;
    splitFirstWord(line, first, remaining);

    while (!isNumber(first)) {

        //cout << "\tARTIST: " << line << endl;
        allMusic.insertArtist(line, title);

        getline(musicFile, line);
        splitFirstWord(line, first, remaining);
    }

    allMusic.insertSong(line, title);

}

//insert an album along with all the artists and the songs
void insertAlbum(fstream& musicFile, MusicLibrary& allMusic) {

    string line;
    getline(musicFile, line);
    string title = line;

    if (title == "") return;

    allMusic.insertAlbum(title);
    insertArtists(title, musicFile, allMusic);

    while (musicFile) {
        getline(musicFile, line);
        if (line == title) return;

        allMusic.insertSong(line, title);
    }
}

//function for load command (takes a filename and the library)
void loadCommand(string& filename, MusicLibrary& allMusic) {
    if (filename == "") {
        filename = "musicdatabase.txt";
    }
    
    fstream musicFile(filename); 
    if (!musicFile.is_open()) {
        cout << "Error: Could not open music library file - nofilehere.txt" << endl;
        return;
    }
    while (musicFile) {
        insertAlbum(musicFile, allMusic);
    }
}

//clear command calling the Music library member funciton
void clearCommand(MusicLibrary& allMusic) {
    allMusic.clear();
}

//default stats calling wihtout -d
void statsCommandDefault(MusicLibrary& allMusic) {
    cout << "Overall Music Library Stats" << endl;
    cout << "===========================" << endl;
    cout << "Total Records: " << allMusic.getTotalRecords() << endl;
    cout << "Total Unique Artists: " << allMusic.getUniqueArtist() << endl;
    cout << "Total Songs: " << allMusic.getTotalSongs() << endl << endl;
}
//printing default stats along with all the data in the library
void statsCommandExtended(MusicLibrary& allMusic) {
    statsCommandDefault(allMusic);
    allMusic.print();
    cout << endl;
}

//search for a substring in all songs and returna a set of all albums
set<string> searchForSongs(string searchItems, MusicLibrary& allMusic) {
    string currSearch;
    splitFirstWord(searchItems, currSearch, searchItems);
    set<string> finalSet;
    char command = ' ';

    while (currSearch != "") {
        if (currSearch[0] == '+' || currSearch[0] == '-') {
            command = currSearch[0];
            currSearch = currSearch.substr(1, currSearch.size() - 1);
        }
        set<string> currRes = allMusic.searchSong(currSearch);

        set<string> newSet;

        if (command == '+') {
            set_intersection(finalSet.begin(), finalSet.end(), currRes.begin(), currRes.end(),
                std::inserter(newSet, newSet.begin()));
            finalSet = newSet;
        }

        else if (command == '-') {
            set_difference(finalSet.begin(), finalSet.end(), currRes.begin(), currRes.end(),
                std::inserter(newSet, newSet.begin()));
            finalSet = newSet;
        }
        else {
            finalSet.insert(currRes.begin(), currRes.end());
        }
        splitFirstWord(searchItems, currSearch, searchItems);
    }
    return finalSet;
}

//search for a given substring in all artists and returna a set of all albums containing the artists
set<string> searchForArtists(string searchItems, MusicLibrary& allMusic) {
    string currSearch;
    splitFirstWord(searchItems, currSearch, searchItems);
    set<string> finalSet;
    char command = ' ';
// searching for the strings with + and - modifiers
    while (currSearch != "") {
        if (currSearch[0] == '+' || currSearch[0] == '-') {
            command = currSearch[0];
            currSearch = currSearch.substr(1, currSearch.size() - 1);
        }
        set<string> currRes = allMusic.searchArtist(currSearch);

        set<string> newSet;

        if (command == '+') {
            set_intersection(finalSet.begin(), finalSet.end(), currRes.begin(), currRes.end(),
                std::inserter(newSet, newSet.begin()));
            finalSet = newSet;
        }
        else if (command == '-') {
            set_difference(finalSet.begin(), finalSet.end(), currRes.begin(), currRes.end(),
                std::inserter(newSet, newSet.begin()));
            finalSet = newSet;
        }
        else {
            finalSet.insert(currRes.begin(), currRes.end());
        }
        splitFirstWord(searchItems, currSearch, searchItems);
    }
    return finalSet;
}

//search for a given substring in all albums and returna a set of all albums containing the substring
set<string> searchForAlbums(string searchItems, MusicLibrary& allMusic) {
    string currSearch;
    splitFirstWord(searchItems, currSearch, searchItems);

    set<string> finalSet;

    char command = ' ';
    while (currSearch != "") {
        if (currSearch[0] == '+' || currSearch[0] == '-') {
            command = currSearch[0];
            currSearch = currSearch.substr(1, currSearch.size() - 1);
        }
        set<string> currRes = allMusic.searchAlbum(currSearch);

        set<string> newSet;

        if (command == '+') {
            set_intersection(finalSet.begin(), finalSet.end(), currRes.begin(), currRes.end(),
                std::inserter(newSet, newSet.begin()));
            finalSet = newSet;
        }

        else if (command == '-') {
            set_difference(finalSet.begin(), finalSet.end(), currRes.begin(), currRes.end(),
                std::inserter(newSet, newSet.begin()));
            finalSet = newSet;
        }
        else {
            finalSet.insert(currRes.begin(), currRes.end());
        }
        splitFirstWord(searchItems, currSearch, searchItems);
    }
    return finalSet;
}

//calling and formatting the search command (helper function used)
void commandSearch(string searchItems, MusicLibrary& allMusic) {
    tolower(searchItems);

    string command;
    splitFirstWord(searchItems, command, searchItems);

    set<string> finalRes;

    if (command == "song") {
        finalRes = searchForSongs(searchItems, allMusic);
    }
    else if (command == "album") {
        finalRes = searchForAlbums(searchItems, allMusic);
    }
    else if (command == "artist") {
        finalRes = searchForArtists(searchItems, allMusic);
    }
    else {
        cout << "Error: Search terms cannot be empty." << endl;
    }

    if (finalRes.size() == 0) {
        cout << "No results found." << endl;
        cout << endl;
        return;
    }

    cout << "Your search results exist in the following albums: " << endl;
    for (string x : finalRes) {
        cout << x << endl;
    }
    cout << endl;
}

int main()
{

    MusicLibrary allMusic;

    string userEntry;
    string command, remains;

    // TODO: Declare any necessary variables for the music libary

    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            clearCommand(allMusic);
        }
        else if (command == "export")
        {
            allMusic.exportCommand(remains);
        }
        else if (command == "load")
        {
            loadCommand(remains, allMusic);

        }
        else if (command == "stats")
        {
            if (remains == "-d") {
                statsCommandExtended(allMusic);
            }
            else statsCommandDefault(allMusic);
        }
        else if (command == "search")
        {
            commandSearch(remains, allMusic);
        }

    } while (command != "exit");

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}