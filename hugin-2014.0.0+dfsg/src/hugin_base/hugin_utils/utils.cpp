// -*- c-basic-offset: 4 -*-

/** @file hugin_utils/utils.cpp
 *
 *  @author Pablo d'Angelo <pablo.dangelo@web.de>
 *
 *  $Id$
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "utils.h"

#ifdef WIN32
    #include <sys/utime.h>
#else
    #include <sys/time.h>
#endif
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <cstdio>
#ifdef _WINDOWS
#define NOMINMAX
#include <windows.h>
#else
#include <hugin_config.h>
#endif
#include <algorithm>

#ifdef __APPLE__
#include <mach-o/dyld.h>  /* _NSGetExecutablePath */
#include <limits.h>       /* PATH_MAX */
#include <libgen.h>       /* dirname */
#endif

#if !defined Hugin_shared || !defined _WINDOWS
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

namespace hugin_utils {
    
#ifdef UNIX_LIKE
std::string GetCurrentTimeString()
{
  char tmp[100];
  struct tm t;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  localtime_r((time_t*)&tv.tv_sec, &t); // is the casting safe?
  strftime(tmp,99,"%H:%M:%S",&t);
  sprintf(tmp+8,".%06ld", (long)tv.tv_usec);
  return tmp;
}
#else
std::string GetCurrentTimeString()
{
    // FIXME implement for Win
    return "";
}
#endif


std::string getExtension(const std::string & basename2)
{
	std::string::size_type idx = basename2.rfind('.');
    // check if the dot is not followed by a \ or a /
    // to avoid cutting pathes.
    if (idx == std::string::npos) {
        // no dot found
		return std::string("");
    }
#ifdef UNIX_LIKE
    // check for slashes after dot
    std::string::size_type slashidx = basename2.find('/', idx);
    if ( slashidx == std::string::npos)
    {
        return basename2.substr(idx+1);
    } else {
        return std::string("");
    }
#else
    // check for slashes after dot
    std::string::size_type slashidx = basename2.find('/', idx);
    std::string::size_type backslashidx = basename2.find('\\', idx);
    if ( slashidx == std::string::npos &&  backslashidx == std::string::npos)
    {
        return basename2.substr(idx+1);
    } else {
		return std::string("");
    }
#endif
}

std::string stripExtension(const std::string & basename2)
{
    std::string::size_type idx = basename2.rfind('.');
    // check if the dot is not followed by a \ or a /
    // to avoid cutting pathes.
    if (idx == std::string::npos) {
        // no dot found
        return basename2;
    }
#ifdef UNIX_LIKE
    std::string::size_type slashidx = basename2.find('/', idx);
    if ( slashidx == std::string::npos)
    {
        return basename2.substr(0, idx);
    } else {
        return basename2;
    }
#else
    // check for slashes after dot
    std::string::size_type slashidx = basename2.find('/', idx);
    std::string::size_type backslashidx = basename2.find('\\', idx);
    if ( slashidx == std::string::npos &&  backslashidx == std::string::npos)
    {
        return basename2.substr(0, idx);
    } else {
        return basename2;
    }
#endif
}

std::string stripPath(const std::string & filename)
{
#ifdef UNIX_LIKE
    std::string::size_type idx = filename.rfind('/');
#else
    std::string::size_type idx1 = filename.rfind('\\');
    std::string::size_type idx2 = filename.rfind('/');
    std::string::size_type idx;
    if (idx1 == std::string::npos) {
        idx = idx2;
    } else if (idx2 == std::string::npos) {
        idx = idx1;
    } else {
        idx = std::max(idx1, idx2);
    }
#endif
    if (idx != std::string::npos) {
//        DEBUG_DEBUG("returning substring: " << filename.substr(idx + 1));
        return filename.substr(idx + 1);
    } else {
        return filename;
    }
}

std::string getPathPrefix(const std::string & filename)
{
#ifdef UNIX_LIKE
    std::string::size_type idx = filename.rfind('/');
#else
    std::string::size_type idx1 = filename.rfind('\\');
    std::string::size_type idx2 = filename.rfind('/');
    std::string::size_type idx;
    if (idx1 == std::string::npos) {
        idx = idx2;
    } else if (idx2 == std::string::npos) {
        idx = idx1;
    } else {
        idx = std::max(idx1, idx2);
    }
#endif
    if (idx != std::string::npos) {
//        DEBUG_DEBUG("returning substring: " << filename.substr(idx + 1));
        return filename.substr(0, idx+1);
    } else {
        return "";
    }
}

std::string doubleToString(double d, int digits)
{
    char fmt[10];
    if (digits < 0) {
        strcpy(fmt,"%f");
    } else {
        std::sprintf(fmt,"%%.%df",digits);
    }
    char c[1024];
    c[1023] = 0;
#ifdef _MSC_VER
    _snprintf (c, 1023, fmt, d);
#else
    snprintf (c, 1023, fmt, d);
#endif

    std::string number (c);

    int l = (int)number.length()-1;

    while ( l != 0 && number[l] == '0' ) {
      number.erase (l);
      l--;
    }
    if ( number[l] == ',' ) {
      number.erase (l);
      l--;
    }
    if ( number[l] == '.' ) {
      number.erase (l);
      l--;
    }
    return number;
}

    void ControlPointErrorColour(const double cperr, 
        double &r,double &g, double &b)
    {
        //Colour change points
        double xp1=5;
        double xp2=10;

        if ( cperr<= xp1) 
        {
            //low error
            r = cperr / xp1;
            g = 0.75;
        }
        else
        {
            r = 1.0;
            g = 0.75 * ( (1.0-std::min<double>(cperr-xp1,(xp2-xp1))/(xp2-xp1)));
        } 
        b = 0.0;
    }

bool FileExists(const std::string& filename)
{
    std::ifstream ifile(filename.c_str());
    return !ifile.fail();
}

std::string GetAbsoluteFilename(const std::string& filename)
{
#ifdef _WINDOWS
    char fullpath[_MAX_PATH];
    _fullpath(fullpath,filename.c_str(),_MAX_PATH);
    return std::string(fullpath);
#else
    //realpath works only with existing files
    //so as work around we create the file first, call then realpath 
    //and delete the temp file
    /** @TODO replace realpath with function with works without this hack */
    bool tempFileCreated=false;
    if(!FileExists(filename))
    {
        tempFileCreated=true;
        std::ofstream os(filename.c_str());
        os.close();
    };
    char *real_path = realpath(filename.c_str(), NULL);
    std::string absPath;
    if(real_path!=NULL)
    {
        absPath=std::string(real_path);
        free(real_path);
    }
    else
    {
        absPath=filename;
    };
    if(tempFileCreated)
    {
        remove(filename.c_str());
    };
    return absPath;
#endif
};

std::string GetDataDir()
{
#if _WINDOWS
    char buffer[MAX_PATH];//always use MAX_PATH for filepaths
    GetModuleFileName(NULL,buffer,sizeof(buffer));
    std::string working_path=(buffer);
    std::string data_path="";
    //remove filename
    std::string::size_type pos=working_path.rfind("\\");
    if(pos!=std::string::npos)
    {
        working_path.erase(pos);
        //remove last dir: should be bin
        pos=working_path.rfind("\\");
        if(pos!=std::string::npos)
        {
            working_path.erase(pos);
            //append path delimiter and path
            working_path.append("\\share\\hugin\\data\\");
            data_path=working_path;
        }
    }
#elif defined MAC_SELF_CONTAINED_BUNDLE
    char path[PATH_MAX + 1];
    uint32_t size = sizeof(path);
    std::string data_path("");
    if (_NSGetExecutablePath(path, &size) == 0)
    {
        data_path=dirname(path);
        data_path.append("/../Resources/xrc/");
    }
#else
    std::string data_path = (INSTALL_DATA_DIR);
#endif
    return data_path;
};

// initialization and wrapup of GPU for GPU remapping
static GLuint GlutWindowHandle;

bool initGPU(int *argcp, char **argv)
{
    glutInit(argcp,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
    GlutWindowHandle = glutCreateWindow("Hugin");

    int err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << argv[0] << ": an error occured while setting up the GPU:" << std::endl;
        std::cerr << glewGetErrorString(err) << std::endl;
        std::cerr << argv[0] << ": Switching to CPU calculation." << std::endl;
        glutDestroyWindow(GlutWindowHandle);
        return false;
    }

    std::cout << argv[0] << ": using graphics card: " << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << std::endl;

    GLboolean has_arb_fragment_shader = glewGetExtension("GL_ARB_fragment_shader");
    GLboolean has_arb_vertex_shader = glewGetExtension("GL_ARB_vertex_shader");
    GLboolean has_arb_shader_objects = glewGetExtension("GL_ARB_shader_objects");
    GLboolean has_arb_shading_language = glewGetExtension("GL_ARB_shading_language_100");
    GLboolean has_arb_texture_rectangle = glewGetExtension("GL_ARB_texture_rectangle");
    GLboolean has_arb_texture_border_clamp = glewGetExtension("GL_ARB_texture_border_clamp");
    GLboolean has_arb_texture_float = glewGetExtension("GL_ARB_texture_float");

    if (!(has_arb_fragment_shader && has_arb_vertex_shader && has_arb_shader_objects && has_arb_shading_language && has_arb_texture_rectangle && has_arb_texture_border_clamp && has_arb_texture_float)) {
        const char * msg[] = {"false", "true"};
        std::cerr << argv[0] << ": extension GL_ARB_fragment_shader = " << msg[has_arb_fragment_shader] << std::endl;
        std::cerr << argv[0] << ": extension GL_ARB_vertex_shader = " << msg[has_arb_vertex_shader] << std::endl;
        std::cerr << argv[0] << ": extension GL_ARB_shader_objects = " << msg[has_arb_shader_objects] << std::endl;
        std::cerr << argv[0] << ": extension GL_ARB_shading_language_100 = " << msg[has_arb_shading_language] << std::endl;
        std::cerr << argv[0] << ": extension GL_ARB_texture_rectangle = " << msg[has_arb_texture_rectangle] << std::endl;
        std::cerr << argv[0] << ": extension GL_ARB_texture_border_clamp = " << msg[has_arb_texture_border_clamp] << std::endl;
        std::cerr << argv[0] << ": extension GL_ARB_texture_float = " << msg[has_arb_texture_float] << std::endl;
        std::cerr << argv[0] << ": This graphics system lacks the necessary extensions for -g." << std::endl;
        std::cerr << argv[0] << ": Switching to CPU calculation." << std::endl;
        glutDestroyWindow(GlutWindowHandle);
        return false;
    }

    return true;
}

bool wrapupGPU()
{
    glutDestroyWindow(GlutWindowHandle);
    return true;
}

} //namespace
