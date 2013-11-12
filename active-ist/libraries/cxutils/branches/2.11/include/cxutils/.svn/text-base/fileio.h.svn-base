////////////////////////////////////////////////////////////////////////////////////
///
///  \file fileio.h
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
#ifndef __CXUTILS_FILE_IO_H
#define __CXUTILS_FILE_IO_H

#include <vector>
#include <string>
#include <fstream>
#include "cxbase.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class FileIO
    ///   \brief Contains functions for creating and deleting files/directories and
    ///   scanning for files.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL FileIO
    {
    public:
        static int CreateDir(const std::string& fname);
        static int DeleteDir(const std::string& fname);
        static int DeleteFiles(const std::string& fname);
        static int DeleteFiles(const std::vector<std::string>& files);
        static std::string GetFileExtension(const std::string& fileName);
        static int GetPath(const std::string& src, std::string& path);
        static int GetFiles(std::vector<std::string>& files,
                            const std::string& ext = "*.*",
                            const std::string& path = "",
                            const bool extractPath = true,
                            const bool recursive = false);
        static int GetDirectories(std::vector<std::string>& dirs,
                                  const std::string& path,
                                  const bool extractPath = false);
        static int GetFilesAndDirectories(std::vector<std::string>& files,
                                          const std::string& path = "",
                                          const bool extractPath = true);
        static FILE* Open(const std::string& name, const std::string& mode = "wt");
        static bool IsDir(const std::string& path);
        static int RenameFile(const std::string& originalName, const std::string& newName);
        static int CopyFileTo(const std::string& existingFile, const std::string& newFile);
        static unsigned int GetFileSizeBytes(const std::string& fileName);
        static unsigned int ParseDelimitedLine(FILE* fp, 
                                               std::vector<std::string>& columns, 
                                               const char delimiter = ',', 
                                               const char end = '\n',
                                               const bool skipBlankEntriesFlag = true);
        static unsigned int ReadLine(FILE* fp, std::string& line);
        static unsigned int ReadLine(std::fstream& str, std::string& line);
        static std::vector<std::string> Tokenize(const std::string& str,
                                                 const std::string& delimter);
        static bool FileExists(const std::string& fname, const bool strict = false);
    };
}

#endif
/*  End of File */
