////////////////////////////////////////////////////////////////////////////////////
///
///  \file pluginloader.cpp
///  \brief Class for loading plug-in libraries.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 25 July 2011
///  <br>Copyright (c) 2011
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
#include "cxutils/plugin/pluginloader.h"
#include <cxutils/fileio.h>
#include <algorithm>

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
    #include <dlfcn.h>
#endif

using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
PluginLoader::PluginLoader()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
PluginLoader::~PluginLoader()
{
    CloseModule();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Opens a shared library (e.g. dll, so) file so that functions and
///   Plugins can be loaded from it.
///
///   \param[in] filename Plugin filename (e.g. libcxutils.dll).
///   \param[in] moduleName Optional name to assign to module, if left blank, then
///                         filename assigned.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool PluginLoader::OpenModule(const std::string& filename,
                              const std::string& moduleName)
{
    bool result = false;

    std::string modName = moduleName;
    std::string realFilename = filename;
    if(modName == "")
    {
        modName = filename;
    }

    if(FileIO::FileExists(realFilename, true) == false)
    {
#ifdef WIN32
        std::string extension = ".dll";
#else
        std::string extension = ".so";
#endif
#ifdef _DEBUG
        realFilename += "_d" + extension;
#else
        realFilename += extension;
#endif
    }

    if(mPluginLibraries.find(modName) != mPluginLibraries.end())
    {
        // Already loaded.
        return true;
    }

#ifdef WIN32
    USES_CONVERSION;
    std::string correctedFilename = realFilename;
    std::replace(correctedFilename.begin(), correctedFilename.end(), '/', '\\');
    HMODULE library = ::LoadLibrary(A2W(correctedFilename.c_str()));
#else
    void* library = dlopen(realFilename.c_str(), RTLD_LAZY);
#endif

    if(library != NULL)
    {
        result = true;
        mPluginLibraries[modName] = (void *)library;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes any loaded module.
///
///   \param[in] moduleName Name of module to close, set to empty string for all.
///
////////////////////////////////////////////////////////////////////////////////////
void PluginLoader::CloseModule(const std::string& moduleName)
{
    std::map<std::string, void*>::iterator lib;
    for(lib = mPluginLibraries.begin(); lib != mPluginLibraries.end(); lib++)
    {
        if(moduleName.empty() || moduleName == lib->first)
        {
#ifdef WIN32
            ::FreeLibrary((HMODULE)lib->second);
#else
            dlclose(lib->second);
#endif
        }
    }
    if(moduleName.empty() == false)
    {
        lib = mPluginLibraries.find(moduleName);
        if(lib != mPluginLibraries.end())
        {
            mPluginLibraries.erase(lib);
        }
    }
    else
    {
        mPluginLibraries.clear();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates an instance of type plugin from a loaded module.
///
///   This method looks up a function with the matching name provided to create
///   an object of type Plugin. Note, due to the nature of plugins, if the
///   loaded DLL is not implemented correctly, this can result in instability.
///
///   \param[in] moduleName Name of module to load plugin instance from.
///   \param[in] pluginCreateFuncName Name of the function used to create
///                                   the plugin instance.
///
///   \return NULL on failure, pointer to Plugin on success.  
///
////////////////////////////////////////////////////////////////////////////////////
Plugin* PluginLoader::CreatePlugin(const std::string& moduleName,
                                   const std::string& pluginCreateFuncName) const
{
    Plugin* plugin = NULL;

    PluginLoader::InitFunction initializer =
                (PluginLoader::InitFunction)LoadPluginFunction(moduleName,
                                                               pluginCreateFuncName);
    if(initializer)
    {
        plugin = (Plugin*)initializer();
    }
    return plugin;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads a pointer to a function in the loaded module.
///
///   Note, due to the nature of plugins, if the
///   loaded DLL is not implemented correctly, this can result in instability.
///
///   \param[in] moduleName Name of module to load plugin instance from.
///   \param[in] pluginCreateFuncName Name of the function used to create
///                                   the plugin instance.
///
///   \return NULL on failure, pointer to function on success.
///
////////////////////////////////////////////////////////////////////////////////////
void* PluginLoader::LoadPluginFunction(const std::string& moduleName,
                                       const std::string& pluginFuncName) const
{
    std::map<std::string, void*>::const_iterator plug;
    plug = mPluginLibraries.find(moduleName);

#ifdef WIN32
    DWORD dw = 0;
    FARPROC initializer;
#else
    void* initializer;
#endif
    if(plug != mPluginLibraries.end())
    {
#ifdef WIN32
        USES_CONVERSION;
        initializer = ::GetProcAddress((HMODULE)plug->second, pluginFuncName.c_str());
        dw = GetLastError();
#else
        initializer = dlsym(plug->second, pluginFuncName.c_str());
#endif
        return (void *)initializer;
    }
    return NULL;
}


/** Example function for creating a plugin. */
CxUtils::Plugin* CreateCxUtilsPluginExample() 
{ 
    return new Plugin(); 
}

/*  End of File */
