////////////////////////////////////////////////////////////////////////////////////
///
///  \file filefinder.cpp
///  \brief Class for locating files and resources.
///
///  <br>Author(s): Chris Constable
///  <br>Created: 27 July 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: cconstab@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#include "cxutils/filefinder.h"
#include "cxutils/fileio.h"
#include <algorithm>
#include <string>

using namespace std;
using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Default Constructor
///
////////////////////////////////////////////////////////////////////////////////////
FileFinder::FileFinder()
{ 
    mRecursionFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
///   \param[in] iniFile String of the path to ini file.
///
////////////////////////////////////////////////////////////////////////////////////
FileFinder::FileFinder(const std::string& iniFile)
{ 
    mRecursionFlag = true;
    LoadIniFile(iniFile);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Default Destructor
///
////////////////////////////////////////////////////////////////////////////////////
FileFinder::~FileFinder()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a resource path to the class's path list.
///
///   \param[in] path String of the path.
///
///   \return True if valid path, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool FileFinder::AddResourcePath(const std::string& path)
{
    if(FileIO::IsDir(path))
    {
        if(find(mResourcePaths.begin(), mResourcePaths.end(), path) == mResourcePaths.end())
        {
            mResourcePaths.push_back(path);
        }        
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds resource paths to the class's path list read in from an
///          ini file.
///
///   \param[in] iniFile String of the path to the ini file.
///
////////////////////////////////////////////////////////////////////////////////////
void FileFinder::LoadIniFile(const std::string& iniFile)
{
    FILE* fin = FileIO::Open(iniFile, "r");
    if(fin)
    {
        string line;
        while(FileIO::ReadLine(fin, line))
        {
            replace(line.begin(), line.end(),'\\','/');
            if(line[line.size()-1] != '/')
            {
                line.append("/");
            }
            AddResourcePath(line); 
        }
        fclose(fin);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Removes a resource path from the class's path list.
///
///   \param[in] path String of the path.
///
///   \return True if valid path, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool FileFinder::RemoveResourcePath(const std::string& path)
{
    vector<string>::iterator pos = find(mResourcePaths.begin(), mResourcePaths.end(), path);
    if(pos != mResourcePaths.end())
    {
        mResourcePaths.erase(pos);
        return true;
    }        
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The value of the recursion flag.
///
////////////////////////////////////////////////////////////////////////////////////
bool FileFinder::GetRecursionFlag() const
{
    return mRecursionFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set's the recursion flag.  If enabled, the
///   finder will search recursively for a file. Recursion is
///   on by default.
///
///   \param[in] recursionFlag Bool value of flag.
///
////////////////////////////////////////////////////////////////////////////////////
void FileFinder::SetRecursionFlag(const bool recursionFlag)
{
    mRecursionFlag = recursionFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Using the file name, returns the full path of the file.
///
///   \param[in] fileName String of the file name.
///
///   \return string containing the full path if successful, "" otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
std::string FileFinder::GetFilePath(const std::string& fileName) const
{
    string filePath;
    std::string justName;
    // Check immediate directory
    if(FileIO::FileExists(fileName))
    {
        return fileName;
    }
    std::string fcopy = fileName;
    for(unsigned int i = 0; i < (unsigned int)fcopy.size(); i++)
    {
        if(fcopy[i] == '\\')
        {
            fcopy[i] = '/';
        }        
        if(fcopy[i] == '/')
        {
            justName = fcopy.substr(i+1,fcopy.size());
        }

    }
    int found = -1;
    std::string trimmedUp = fcopy;
    while((found = trimmedUp.find("../")) != string::npos)
    {
        trimmedUp = trimmedUp.substr(found+3,fcopy.size());
    }

    // Check each resource directory
    found = -1;
    vector<string>::const_iterator curPath;
    for(curPath = mResourcePaths.begin(); curPath != mResourcePaths.end(); curPath++)
    {
        std::string trimmed = trimmedUp;
        do
        {
            trimmed = trimmed.substr(found+1,trimmed.size());
            string tempPath = (*curPath) + trimmed;
            if(FileIO::FileExists(tempPath))
            {
                return tempPath;
            }
        }while((found = trimmed.find('/')) != string::npos);

        // If recursion is enabled...
        if(mRecursionFlag)
        {
            GetFilePathHelper((*curPath), justName, filePath);
        }
    }

    return filePath;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Recursive helper for the GetFilePath() method.
///
///   \param[in] rootPath String of the file name.
///   \param[in] fileName String of the file name.
///   \param[out] filePath String found file's path.
///
///   \return string containing the full path if successful, "" otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
void FileFinder::GetFilePathHelper(const std::string& rootPath,
                                   const std::string& fileName, 
                                   std::string &filePath) const
{
    vector<string> subDir;
    vector<string>::iterator curDir;
    if(filePath.empty())
    {
        FileIO::GetDirectories(subDir, rootPath, true);
    }
    for(curDir = subDir.begin(); (curDir != subDir.end()) && filePath.empty(); curDir++)
    {
        std::string complete = rootPath + "/" +(*curDir) + "/" + fileName;
        if(FileIO::FileExists(complete))
        {
            filePath = complete;
            return;
        }
        GetFilePathHelper(rootPath+(*curDir), fileName, filePath);

    }
}

/*  End of File */
