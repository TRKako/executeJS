#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>

using namespace std;

HWND hConsoleWnd = ::GetConsoleWindow();
HMONITOR hMonitor = ::MonitorFromWindow(hConsoleWnd, MONITOR_DEFAULTTONEAREST);

/*//////////////////////////////    FUNCTIONS    //////////////////////////////*/



////////////////FUNCTION TO CHECK IF INDEX EXISTS////////////////
bool checkINDEX(const string& IndexPath) {
    ifstream file(IndexPath);
    if (!file.is_open()) {
        system("title ERR: INDEX_NOT_FOUND");
        return false;
    }
    return true;
}

string getINDEXContent(const string& IndexPath) {
    ifstream file(IndexPath);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

////////////////FUNCTION TO CHECK IF THE ALL REQUIRED FILES ARE THERE////////////////
string readFile(const string& filePath) {
    ifstream file(filePath);
        if (!file.is_open()) {
        	system("title FILE_PATH_NOT_FOUND");
            cerr << "Cannot open '" << filePath << "', please check it's path again";
            system("pause>nul");
            exit(EXIT_FAILURE);
}
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


////////////////FUNCTION TO SEARCH FOR VAR.JSON VALUES////////////////
string extractJsonValue(const string& jsonContent, const string& key) {
    size_t keyPos = jsonContent.find("\"" + key + "\"");
    if (keyPos == string::npos) {
        cerr << "KEY '" << key << "' not found on var.json";
        system("pause>nul");
        exit(EXIT_FAILURE);
    }
    size_t valueStart = jsonContent.find(":", keyPos) + 1;
    size_t valueEnd = jsonContent.find("\"", valueStart + 1);
    return jsonContent.substr(valueStart + 1, valueEnd - valueStart - 1);
}

////////////////LOAD ICON FROM JSON VALUE////////////////
HICON LoadIconFromFile(const std::string& iconPath) {
    return reinterpret_cast<HICON>(LoadImageA(nullptr, iconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));
}



int main() { /*//////////////////////////////    MAIN    //////////////////////////////*/


////////////////WIN RESIZE////////////////
if (hMonitor) {
   MONITORINFO info{sizeof(info)};
    if (::GetMonitorInfo(hMonitor, &info)) {
        int width = 500;
        int height = 200;
        int x = (info.rcWork.left + info.rcWork.right) - width + 10;
        int y = (info.rcWork.top + info.rcWork.bottom) - height + 6;
        ::SetWindowPos(hConsoleWnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOOWNERZORDER);
    }	
}


////////////////GET VALUES FROM VAR.JSON////////////////
    string jsonContent = readFile("var.json");
    string program_name = extractJsonValue(jsonContent, "program_name");
    string rute = extractJsonValue(jsonContent, "rute");
    string icon = extractJsonValue(jsonContent	, "icon");


////////////////DEFAULT ICON IF NOTHING SPECIFIED ON 'ICON' VALUE////////////////
if(icon == "" || icon == " " || icon == "./icons/"){
    icon = "./icons/default.ico";
}


////////////////CUSTOM CONSOLE WIN ICON////////////////
if (hMonitor) {
    HICON hIcon = LoadIconFromFile(icon);
    if (hIcon) {
        ::SendMessage(hConsoleWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
        ::SendMessage(hConsoleWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
    } else {
    	system("title ICON_PATH_NOT_FOUND");
        cerr << "Cannot find path '" << icon << "' to specified icon in var.json:4:10";
		system("pause>nul");
		exit(EXIT_FAILURE);
    }
}


////////////////CHECK IF 'RUTE' HAS ENDS WITH '/' ////////////////
 if (rute.empty()) {
//true
    } else if (rute.back() == '/') {
//true
    } else {
    rute += "/";
 //false
}


////////////////CHECK IF INDEX.JS EXISTS////////////////
if (checkINDEX(rute + "index.js")) {
        //string content = getINDEXContent(rute + "index.js");
    } else {
        cout << "'index.js' not found on '" << rute << "index.js'\n\nPlease check it's path again and make sure that your index.js\nit's on the path specified in var.json:3:10";
        system("pause>nul");
        exit(EXIT_FAILURE);
}


////////////////PROGRAM NAME////////////////
if(program_name == "" || program_name == " "){
    program_name = "Program Name";
}

////////////////RUN INDEX////////////////
if(rute == "" || rute == " "){
  	
string command = "@echo off && title " + program_name + " && cls && node index.js";
    system(command.c_str());
    system("@echo off && echo: && echo [Launcher Terminated] && pause>nul");
    
} else {
		
string command = "@echo off && title " + program_name + " && cls && cd " + rute + " && node index.js";
    system(command.c_str());
    system("@echo off && echo: && echo [Launcher Terminated] && pause>nul");   

	}
}