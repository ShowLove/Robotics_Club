////////////////////////////////////////////////////////////////////////////////////
///
///  \file fileio.cpp
///  \brief Functions to create directories and scan for files.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 June 2007
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
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
#include "cxutils/fileio.h"

#ifdef WIN32
    #include <windows.h>
    #ifndef MINGW
        #include <atlbase.h>
    #else
        #define USES_CONVERSION
        #define A2W(x) (x)
        #define W2A(x) (x)
    #endif
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <stdlib.h>
    #include <string.h>
    #include <dirent.h>    // NON-STANDARD ANSI C library, but common
    #include <fnmatch.h>
#endif

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

//using namespace std;
using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a directory with the filename.  The directory is
///   created relative to the working directory of the program, unless an
///   absolute path is included in the file name (ex: C:/temp).
///
///   \param fname Name of the directory.
///
///   \return 1 if created, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::CreateDir(const std::string& fname)
{
    int result = 0;
    std::string pathFolder = fname;
    for(unsigned int i = 0; i < (unsigned int)pathFolder.size(); i++)
    {
        if(pathFolder.c_str()[i] == '\\')
        {
            ((char *)pathFolder.c_str())[i] = '/';
        }
    }
    std::vector<std::string> tokens = CxUtils::FileIO::Tokenize(pathFolder, "/");
    if(tokens.size() > 1)
    {
        std::string total;
        for(unsigned int i = 0; i < (unsigned int)tokens.size(); i++)
        {
            total += tokens[i];
            total += "/";
#ifdef WIN32
            USES_CONVERSION;
            result = ::CreateDirectory(A2W(total.c_str()), 0);
            if(result != 1)
                result = 0;
#else
            //  Default file permissions on the directory to 755.
            result = !::mkdir(total.c_str(), 0755);
#endif

        }
    }
    else
    {
#ifdef WIN32
        USES_CONVERSION;
        result = ::CreateDirectory(A2W(fname.c_str()), 0);
        if(result != 1)
            result = 0;
#else
        //  Default file permissions on the directory to 755.
        result = !::mkdir(fname.c_str(), 0755);
#endif
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes a directory with the filename.  The directory is
///   created relative to the working directory of the program, unless an
///   absolute path is included in the file name (ex: C:/temp).
///
///   \param fname Name of the directory.
///
///   \return 1 if deleted, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::DeleteDir(const std::string& fname)
{
    int result = 0;
#ifdef WIN32
    USES_CONVERSION;
    result = ::RemoveDirectory(A2W(fname.c_str()));
    if(result != 1)
        result = 0;
#else
    result = !::rmdir(fname.c_str());
#endif
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes a file based on name.  The file is
///   relative to the working directory of the program, unless an
///   absolute path is included in the file name (ex: C:/temp.log.txt).
///
///   \param fname Name of the file.
///
///   \return 1 if deleted, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::DeleteFiles(const std::string& fname)
{
    return !remove(fname.c_str());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes files based on names.  The files are treated
///   relative to the working directory of the program, unless an
///   absolute path is included in the file name (ex: C:/temp.log.txt).
///
///   \param files Vector of file names of files to delete.
///
///   \return Number of files deleted, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::DeleteFiles(const std::vector<std::string>& files)
{
    int result = 0;
    for(unsigned int i = 0; i < (unsigned int)files.size(); i++)
    {
        if(!DeleteFiles(files[i]))
            result++;
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the file extension from a filename.
///
///   \param fileName The name of the file to parse.
///
///   \return The extension of the file or "" otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
std::string FileIO::GetFileExtension(const std::string& fileName)
{
    if(fileName.find_last_of(".") != std::string::npos)
    {
        return fileName.substr(fileName.find_last_of(".") + 1);
    }

    return "";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the file path from a string.
///
///   \param src Source string with path information.
///   \param path The extracted path.
///
///   \return 1 if path retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::GetPath(const std::string& src, std::string& path)
{
    path.clear();
    if(src.size() > 0 )
    {

        //  Find out the number of characters after the last
        //  delimiter.
        unsigned int fnamesize = 0;
        const char *str = src.c_str();
        for(int i = (int)(src.size() - 1); i >= 0; i--)
        {
            if(str[i] == '\\' || str[i] == '/' )
            {
                break;
            }
            fnamesize++;
        }
        //  Now add to path up to file name.
        for(unsigned int i = 0; i < (unsigned int)(src.size() - fnamesize); i++)
            path += str[i];
    }

    if(path.size() > 0)
        return 1;
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the list of all files in a directory.
///
///   \param files Vector of files found in directory.
///   \param ext The search pattern for files (*.*, *.xml, etc).
///   \param path The path (directory to search in).
///   \param extractPath If true, then the path is extracted from the file names.
///   \param recursive If true, method will get files from sub-directories also.
///
///   \return Number of files loaded.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::GetFiles(std::vector<std::string>& files,
                     const std::string& ext,
                     const std::string& path,
                     const bool extractPath,
                     const bool recursive)
{
    int result = 0;
    files.clear();
    std::string dir;
    std::string findPattern;

    dir = path;
    if( dir.empty() )
    {
        findPattern = ext;
    }
    else
    {
        // If there is no slash at the end of
        // the path string, add one.
        if( dir.c_str()[dir.length() - 1] != '/' &&
            dir.c_str()[dir.length() - 1] != '\\' )
        {
            dir += '/';
        }
        findPattern = dir + ext;
    }

    // Now that we have our find pattern, get the files.

#ifdef WIN32
    ::WIN32_FIND_DATA foundFiles;
    ::HANDLE hFind;

    USES_CONVERSION;

    hFind = ::FindFirstFile(A2W(findPattern.c_str()), &foundFiles);

    if( hFind != INVALID_HANDLE_VALUE )
    {
        do
        {
            if( !(foundFiles.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                std::string item;
                if( !extractPath )
                {
                    item = dir;
                    item += W2A(foundFiles.cFileName);
                }
                else
                {
                    item = W2A(foundFiles.cFileName);
                }
                files.push_back(item);
                result++;
            }
        }
        while( ::FindNextFile(hFind, &foundFiles) );
    }
#else
    DIR *directory = opendir(path.length() == 0 ? "." : path.c_str());
    if (directory)
    {
        dirent *d = readdir(directory);
        while (d)
        {
            if (!fnmatch(ext.c_str(), d->d_name, 0))
            {
                if(!extractPath)
                {
                    files.push_back(dir + std::string(d->d_name));
                }
                else
                {
                    files.push_back(d->d_name);
                }
                result++;
            }
            d = readdir(directory);
        }
        closedir(directory);
    }
#endif

    if(recursive)
    {
        std::vector<std::string> directories;
        FileIO::GetDirectories(directories, path, false);
        for(unsigned int i = 0; i < (unsigned int)directories.size(); i++)
        {
            std::vector<std::string> rfiles;
            if(GetFiles(rfiles, ext, directories[i], false, true))
            {
                for(unsigned int j = 0; j < (unsigned int)rfiles.size(); j++)
                {
                    if(extractPath)
                    {
                        rfiles[j].erase(0, dir.size());
                    }
                    files.push_back(rfiles[j]);
                }
            }
        }
    }

    return (int)files.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the list of all directories in a folder.
///
///   \param dirs Vector of all directories found.
///   \param path The path (directory to search in).
///   \param extractPath If true, then the path is extracted from the file names.
///
///   \return Number of files loaded.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::GetDirectories(std::vector<std::string>& dirs,
                           const std::string& path,
                           const bool extractPath)
{
    int result = 0;
    dirs.clear();
    std::string findPattern;

    findPattern = path;
    // Make all naming consistent with forward slashes.
    for(unsigned int i = 0; i < (unsigned int)(findPattern.size()); i++)
    {
        if(findPattern.c_str()[i] == '\\')
        {
            ((char *)(findPattern.c_str()))[i] = '/';
        }
    }

    if(findPattern.size() > 0)
    {
        if(findPattern.c_str()[findPattern.size() - 1] != '\\' &&
           findPattern.c_str()[findPattern.size() - 1] != '/')
        {
            findPattern.push_back('/');
        }
    }

    // Now that we have our find pattern, get the dirs.

#ifdef WIN32
    ::WIN32_FIND_DATA foundFiles;
    ::HANDLE hFind;

    USES_CONVERSION;
    std::string pattern = findPattern;
    pattern += "*.*";
    hFind = ::FindFirstFile(A2W(pattern.c_str()), &foundFiles);

    if( hFind != INVALID_HANDLE_VALUE )
    {
        do
        {
            if( foundFiles.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                std::string dir = W2A(foundFiles.cFileName);
                if(dir != "." && dir != "..")
                {
                    std::string item;
                    if( !extractPath )
                    {
                        item = findPattern;
                        item += dir;
                    }
                    else
                    {
                        item = dir;
                    }
                    dirs.push_back(item);
                    result++;
                }
            }
        }
        while( ::FindNextFile(hFind, &foundFiles) );
    }
#else
    DIR *directory = opendir(path.length() == 0 ? "." : path.c_str());
    if (directory)
    {
        dirent *d = readdir(directory);
        while (d)
        {
            std::string dir = d->d_name;
            if(dir != "." && dir != "..")
            {
                std::string test = findPattern;
                test += d->d_name;
                DIR* testDir = opendir(test.c_str());
                //cout << test << endl;
                if(testDir)
                {
                    std::string item;
                    if( !extractPath )
                    {
                        item = test;
                    }
                    else
                    {
                        item = dir;
                    }
                    dirs.push_back(item);
                    result++;
                }
            }
            d = readdir(directory);
        }
        closedir(directory);
    }
#endif

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the list of all data in a directory.
///
///   \param files Vector of all data found.
///   \param path The path (directory to search in).
///   \param extractPath If true, then the path is extracted from the file names.
///
///   \return Number of files loaded.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::GetFilesAndDirectories(std::vector<std::string>& files,
                                   const std::string& path,
                                   const bool extractPath)
{
    int result = 0;
    files.clear();
    std::string findPattern;

    findPattern = path;
    // Make all naming consistent with forward slashes.
    for(unsigned int i = 0; i < (unsigned int)(findPattern.size()); i++)
    {
        if(findPattern.c_str()[i] == '\\')
        {
            ((char *)(findPattern.c_str()))[i] = '/';
        }
    }
    if(findPattern.size() > 0 && findPattern[findPattern.size() - 1] != '/')
    {
        findPattern.push_back('/');
    }

    // Now that we have our find pattern, get the dirs.

#ifdef WIN32
    ::WIN32_FIND_DATA foundFiles;
    ::HANDLE hFind;

    USES_CONVERSION;
    std::string pattern = findPattern;
    pattern += "*.*";
    hFind = ::FindFirstFile(A2W(pattern.c_str()), &foundFiles);

    if( hFind != INVALID_HANDLE_VALUE )
    {
        do
        {
            std::string dir = W2A(foundFiles.cFileName);
            if(dir != "." && dir != "..")
            {
                std::string item;
                if( !extractPath )
                {
                    item = findPattern;
                    item += dir;
                }
                else
                {
                    item = dir;
                }
                files.push_back(item);
                result++;
            }
        }
        while( ::FindNextFile(hFind, &foundFiles) );
    }
#else
    DIR *directory = opendir(path.length() == 0 ? "." : path.c_str());
    if (directory)
    {
        dirent *d = readdir(directory);
        while (d)
        {
            std::string dir = d->d_name;
            if(dir != "." && dir != "..")
            {
                std::string test = findPattern;
                test += d->d_name;

                std::string item;
                if( !extractPath )
                {
                    item = test;
                }
                else
                {
                    item = dir;
                }
                files.push_back(item);
                result++;
            }
            d = readdir(directory);
        }
        closedir(directory);
    }
#endif

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a file with options, but creates any directories needed also.
///
///   \param[in] name File name with directory paths (e.g. settings/my_file.csv).
///   \param[in] mode Mode options to pass to fopen on file creation.
///
///   \return Valid file pointer on success, NULL on failure.
///
////////////////////////////////////////////////////////////////////////////////////
FILE* FileIO::Open(const std::string& name, const std::string& mode)
{
    std::vector<std::string> tokens = Tokenize(name, "/");
    if(tokens.size() == 0)
    {
        tokens = Tokenize(name, "\\");
    }
    if(tokens.size() > 0 && strchr(mode.c_str(), 'w'))
    {
        std::string dir;
        for(int i = 0; i < (int)tokens.size() - 1; i++)
        {
            dir += tokens[i];
            dir += "/";
            CreateDir(dir);
        }
    }
    return fopen(name.c_str(), mode.c_str());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Check if a directory exists.
///
///   \param[in] path Directory to check for.
///
///   \return True if the path is a valid directory, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool FileIO::IsDir(const std::string& path)
{
#ifdef WIN32
    USES_CONVERSION;
    DWORD attr = ::GetFileAttributes(A2W(path.c_str()));
    if(attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
    {
        return true;
    }
#else
    DIR* testDir = opendir(path.c_str());
    //cout << test << endl;
    if(testDir)
    {
        return true;
    }
#endif
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies a file and renames it.
///
///   \param[in] originalName The file to change the name of.
///   \param[in] newName The new file name.
///
///   \return 1 on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::RenameFile(const std::string& originalName, const std::string& newName)
{
    return !rename(originalName.c_str(), newName.c_str());
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies a file and renames it.
///
///   \param[in] existingFile The file to copy.
///   \param[in] newFile The new copied file (any existing file is replaced).
///
///   \return 1 on success, false on failure./home/dbarber/Software/ACTIVE-IST/libraries/cxutils/trunk/1.0/lib
///
////////////////////////////////////////////////////////////////////////////////////
int FileIO::CopyFileTo(const std::string& existingFile, const std::string& newFile)
{
    int result = 0;
#ifdef WIN32
    USES_CONVERSION;
    if(::CopyFile(A2W(existingFile.c_str()), A2W(newFile.c_str()), false))
    {
        result = 1;
    }
#else
    std::ifstream ifs(existingFile.c_str(), std::ios::binary);
    std::ofstream ofs(newFile.c_str(), std::ios::binary);
    if(ifs.is_open() && ofs.is_open())
    {
        ofs << ifs.rdbuf();
        result = 1;
        ifs.close();
        ofs.close();
    }
#endif
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the size of a file in bytes.
///
///   \param[in] filename Name of the file to get the size of.
///
///   \return Number of bytes contained in the file, 0 if none or file doesn't
///           exist.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int FileIO::GetFileSizeBytes(const std::string& filename)
{
    std::ifstream ifs(filename.c_str(), std::ios::binary);
    if(ifs.is_open())
    {
        return (unsigned int)ifs.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in);
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads a line in a file up to the end of file or the provided
///          ending character (default is carriage return).
///
///   \param[in] fp File pointer to read from.
///   \param[out] columns The delimited string data read.
///   \param[in] delimiter The delimter for separation of data.
///   \param[in] end Ending character to signal stop reading.
///   \param[in] skipBlankEntriesFlag If true, blank entries (e.g. ",,") that
///              have not characters between delimeter are not added to
///              output vector.  If false, empty strings are placed into
///              output columns vector.
///
///   \return Number of elements read from the line, 0 if nothing.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int FileIO::ParseDelimitedLine(FILE* fp,
                                        std::vector<std::string>& columns,
                                        const char delimiter,
                                        const char end,
                                        const bool skipBlankEntriesFlag)
{
    char ch;
    std::string str;

    str.reserve(512);
    columns.clear();
    while(fp && !feof(fp))
    {
        ch = fgetc(fp);
        if(delimiter == ch || end == ch)
        {
            // Remove space at the beginning of the string, then
            // push onto the list of strings.
            std::string::iterator c;
            c = str.begin();
            while( c != str.end() && (*c) == ' ')
            {
                c = str.erase(c);
            }
            if(str.size() > 0)
            {
                columns.push_back(str);
            }
            else if(!skipBlankEntriesFlag)
            {
                columns.push_back("");
            }
            str.clear();
            if(end == ch)
            {
                break;
            }
        }
        else
        {
            str.push_back(ch);
        }
    }

    return (unsigned int)columns.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads a line from a FILE pointer adding it to a string, reading up
///          to a newline character.
///
///   \param[in] fp File pointer to read from.
///   \param[out] line Line read from file (doesn't include carriage return).
///
///   \return Number of characters read.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int FileIO::ReadLine(FILE* fp,
                              std::string& line)
{
    char ch;
    line.reserve(512);
    line.clear();
    while(fp && !feof(fp))
    {
        ch = fgetc(fp);
        if(ch == '\n')
        {
            break;
        }
        else
        {
            line.push_back(ch);
        }
    }

    return (unsigned int)line.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads a line from a fstream adding it to a string, reading up
///          to a newline character.
///
///   \param[in] str File stream operator.
///   \param[out] line Line read from file (doesn't include carriage return).
///
///   \return Number of characters read.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int FileIO::ReadLine(std::fstream& str,
                              std::string& line)
{
    char ch;
    line.reserve(512);
    line.clear();
    while(str.eof() == false && str.good())
    {
        str.get(ch);
        if(ch == '\n')
        {
            break;
        }
        else
        {
            line.push_back(ch);
        }
    }

    return (unsigned int)line.size();
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Tokenize a string.
///
///   \param[in] str String to get tokens from.
///   \param[in] delimiters Delimter for tokens.
///
///   \return Vector of token strings.
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> FileIO::Tokenize(const std::string& str,
                                          const std::string& delimiters)
{
    std::string client = str;
    std::vector<std::string> result;

    while (!client.empty())
    {
        std::string::size_type dPos = client.find_first_of( delimiters );

        if (dPos == std::string::npos)
        {
            std::string::size_type ePos = 0;
            while(client.c_str()[ePos] == ' ')
            {
                ePos++;
            }
            result.push_back(client.substr(ePos, std::string::npos));
            return result;
        }
        while(client.substr(dPos).substr(0, delimiters.size()) != delimiters)
        {
            dPos++;
            if(dPos >= client.size())
            {
                break;
            }
        }
        std::string element = client.substr(0, dPos);
        std::string::size_type ePos = 0;
        while(element.c_str()[ePos] == ' ')
        {
            ePos++;
        }
        element = element.substr(ePos, std::string::npos);
        result.push_back(element);
        if(dPos + delimiters.length() >= client.size())
        {
            break;
        }
        client = client.substr(dPos + delimiters.length());
    }
    if (client.empty())
    {
        result.push_back("");
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if a file exists.
///
///   \param[in] fname Name of the file.
///
///   \return True if exists, false if does not.
///
////////////////////////////////////////////////////////////////////////////////////
bool FileIO::FileExists(const std::string& fname)
{
    std::vector<std::string> tokens;
    std::string fcopy = fname;
    unsigned int last = 0;
    for(unsigned int i = 0; i < (unsigned int)fcopy.size(); i++)
    {
        if(fcopy[i] == '\\')
        {
            fcopy[i] = '/';
        }
        if(fcopy[i] == '/')
        {
            last = i;
        }
    }

    std::string path = fcopy;
    if(last > 0 && last + 1 < (unsigned int)fcopy.size())
    {
        path.resize(last);
    }
    if(last == 0)
    {
        path = ".";
    }
    if(FileIO::GetFiles(tokens,
                        "*.*",
                        path,
                        false,
                        true))
    {
        std::vector<std::string>::iterator f;
        for(f = tokens.begin();
            f != tokens.end();
            f++)
        {
            if(strstr((*f).c_str(), fcopy.c_str()) != 0)
            {
                return true;
            }
        }
    }

    FILE* fp = fopen(fname.c_str(), "r");

    if (fp)
    {
        fclose(fp);
        return true;
    }

    return false;
}
/*  End of File */
