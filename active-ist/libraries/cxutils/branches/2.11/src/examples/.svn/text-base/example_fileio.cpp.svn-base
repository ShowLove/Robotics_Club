////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_fileio.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  FileIO functions for creating directories, and getting
///  directory contents.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 28 April 2007
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
#include <iostream>
#include <vector>
#include "cxutils/cxutils.h"

#ifdef WIN32
#include <conio.h>
#endif

using namespace std;
using namespace CxUtils;


int main(int argc, char **argv)
{
    if(FileIO::CreateDir("FileIO_DirCreate")) 
    {
        cout << "Created directory.\n";
        if(FileIO::DeleteDir("FileIO_DirCreate"))
            cout << "Deleted directory.\n";
    }

    FILE *fp = fopen("FileIO_TestFile.txt", "w+t");
    if(fp) 
    {
        cout << "Created file to test delete on.\n";
        FileIO::FileExists("FileIO_TestFile.txt");
        fclose(fp);        
        if(FileIO::DeleteFiles("FileIO_TestFile.txt"))
            cout << "Deleted file!\n";
    }

    string file = "/this/is/a/test/path/file.txt";
    string path;

    cout << "The path for the file: " << file << " is ";
    FileIO::GetPath(file, path);
    cout << path << endl;

    file = "\\this\\is\\another\\test\\path\\file.txt";

    cout << "The path for the file: " << file << " is ";
    FileIO::GetPath(file, path);
    cout << path << endl;

    // Load all files on root
    cout << "Files on /\n";
    std::vector<std::string> files;
#ifdef WIN32
    FileIO::GetFiles(files, "*.*", "C:/", false);
#else
    FileIO::GetFiles(files, "*.*", "/", false);
#endif
    for( unsigned int i = 0; i < (unsigned int)files.size(); i++ )
    {
        cout << files[i] << endl;
    }

    // Load all directories on root
    cout << "Directories on /\n";
#ifdef WIN32
    FileIO::GetDirectories(files, "C:/", false);
#else
    FileIO::GetDirectories(files, "/", false);
#endif
    for( unsigned int i = 0; i < (unsigned int)files.size(); i++ )
    {
        cout << files[i] << endl;
    }

    // Load all DATA on root
    cout << "Data on /\n";
#ifdef WIN32
    FileIO::GetFilesAndDirectories(files, "C:/", false);
#else
    FileIO::GetFilesAndDirectories(files, "/", false);
#endif
    cout << "Directories: \n";
    for( unsigned int i = 0; i < (unsigned int)files.size(); i++ )
    {
        if(FileIO::IsDir(files[i]))
        {
            cout << files[i] << endl;
        }
        else
        {
            //cout << files[i] << endl;
        }
    }
    cout << "Files: \n";
    for( unsigned int i = 0; i < (unsigned int)files.size(); i++ )
    {
        if(FileIO::IsDir(files[i]))
        {
            //cout << files[i] << endl;
        }
        else
        {
            cout << files[i] << endl;
        }
    }

    FileIO::CopyFileTo("calculon640.jpg", "fileio_calculon640.jpg");

    FileIO::GetFiles(files, "*.xml", "C:/Active/applications/ocu/3.0/bin/missions/", true, true);

    for( unsigned int i = 0; i < (unsigned int)files.size(); i++ )
    {
        cout << files[i] << endl;
    }

    return 0;
}


/*  End of File */
