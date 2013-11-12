////////////////////////////////////////////////////////////////////////////////////
///
///  \file filefinder.h
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
#ifndef __CXUTILS_FILE_FINDER_H
#define __CXUTILS_FILE_FINDER_H

#include <vector>
#include <string>
#include "cxbase.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class FileFinder
    ///   \brief Class used for standardizing the retrieval of resource files.
    ///
    ///   The FileFinder class reads in a "resource_paths.ini" file when instantiated.
    ///   This file contains path directories that the FileFinder will use (recursively)
    ///   to search for files, given a file name.
    ///
    ///   The FileFinder does not open files, it simply returns the correct path of the
    ///   file requested. Resource paths may be read in from a specified file or entered
    ///   manually.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL FileFinder
    {
    public:
        FileFinder();
        FileFinder(const std::string& iniFile);
        ~FileFinder();
        bool AddResourcePath(const std::string& path);
        void LoadIniFile(const std::string& iniFile = "resource_paths.ini"); 
        bool RemoveResourcePath(const std::string& path);
        bool GetRecursionFlag() const;
        void SetRecursionFlag(const bool recursionFlag);
        std::string GetFilePath(const std::string& fileName) const;
    private:
        void GetFilePathHelper(const std::string& rootPath, 
                               const std::string& fileName, 
                               std::string &filePath) const; // Recursive helper for GetFilePath().
        std::vector<std::string> mResourcePaths;    ///< Stores all resource paths.
        bool mRecursionFlag;                        ///< Signals whether or not we recursively search resource paths.
    };
}
#endif

/* End of File*/
