// Copyright (c) 2017, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory (LLNL).
// Written by Cosmin G. Petra, petra1@llnl.gov.
// LLNL-CODE-742473. All rights reserved.
//
// This file is part of HiOp. For details, see https://github.com/LLNL/hiop. HiOp 
// is released under the BSD 3-clause license (https://opensource.org/licenses/BSD-3-Clause). 
// Please also read “Additional BSD Notice” below.
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// i. Redistributions of source code must retain the above copyright notice, this list 
// of conditions and the disclaimer below.
// ii. Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the disclaimer (as noted below) in the documentation and/or 
// other materials provided with the distribution.
// iii. Neither the name of the LLNS/LLNL nor the names of its contributors may be used to 
// endorse or promote products derived from this software without specific prior written 
// permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
// SHALL LAWRENCE LIVERMORE NATIONAL SECURITY, LLC, THE U.S. DEPARTMENT OF ENERGY OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
// AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Additional BSD Notice
// 1. This notice is required to be provided under our contract with the U.S. Department 
// of Energy (DOE). This work was produced at Lawrence Livermore National Laboratory under 
// Contract No. DE-AC52-07NA27344 with the DOE.
// 2. Neither the United States Government nor Lawrence Livermore National Security, LLC 
// nor any of their employees, makes any warranty, express or implied, or assumes any 
// liability or responsibility for the accuracy, completeness, or usefulness of any 
// information, apparatus, product, or process disclosed, or represents that its use would
// not infringe privately-owned rights.
// 3. Also, reference herein to any specific commercial products, process, or services by 
// trade name, trademark, manufacturer or otherwise does not necessarily constitute or 
// imply its endorsement, recommendation, or favoring by the United States Government or 
// Lawrence Livermore National Security, LLC. The views and opinions of authors expressed 
// herein do not necessarily state or reflect those of the United States Government or 
// Lawrence Livermore National Security, LLC, and shall not be used for advertising or 
// product endorsement purposes.

#ifndef HIOP_OPTIONS
#define HIOP_OPTIONS

#include "hiopLogger.hpp"

#include <string>
#include <vector>
#include <map>

namespace hiop
{
class hiopLogger;
 
class hiopOptions
{
public:
  hiopOptions(const char* szOptionsFilename=NULL);
  virtual ~hiopOptions();

  //Seters for options values that should be self explanatory with the exception of the last parameter.
  //
  //Passing 'setFromFile' with non-default, 'true' value is for expert use-only. It indicates that the option 
  //value comes from the options file (hiop.options) and will overwrite any options set at runtime by the 
  //user's code. However, passing 'setFromFile' with 'true' during runtime is perfectly fine and will 
  //conviniently "overwrite the overwriting" file options  

  virtual bool SetNumericValue (const char* name, const double& value, const bool& setFromFile=false);
  virtual bool SetIntegerValue(const char* name, const int& value, const bool& setFromFile=false);
  virtual bool SetStringValue (const char* name,  const char* value, const bool& setFromFile=false);
  
  virtual double      GetNumeric(const char* name) const;
  virtual int         GetInteger(const char* name) const;
  virtual std::string GetString (const char* name) const;

  void SetLog(hiopLogger* log_) { log=log_; ensureConsistence(); }
  virtual void print(FILE* file, const char* msg=NULL) const;
protected:
  /* internal use only */

  void registerNumOption(const std::string& name, double defaultValue, double rangeLo, double rangeUp, const char* description);
  void registerIntOption(const std::string& name, int    defaultValue, int    rangeLo, int    rangeUp, const char* description);
  //void registerBooOption(const std::string& name, bool defaultValue);
  void registerStrOption(const std::string& name, const std::string& defaultValue, const std::vector<std::string>& range, const char* description);
  void registerOptions();

  //sets the (name, value) pair accordingly to the type registered in mOptions, or prints an warning message and leaves
  //the 'name' option to the default value. This method is for internal use.
  bool setNameValuePair(const std::string& name, const std::string& value);

  void loadFromFile(const char* szFilename);

  void ensureConsistence();

  void log_printf(hiopOutVerbosity v, const char* format, ...);

  struct _O { // option entry
    _O(const char* description) : descr(description), specifiedInFile(false) {};
    virtual ~_O() {};
    std::string descr;
    bool specifiedInFile;
    virtual void print(FILE* f) const =0;
  };
  struct _OInt : public _O { 
    _OInt(int    v, int    low,    int upp, const char* description) : _O(description), val(v), lb(low), ub(upp) {}; 
    int    val, lb, ub; 
    void print(FILE* f) const;
  };
  struct _ONum : public _O { 
    _ONum(double v, double low, double upp, const char* description) : _O(description), val(v), lb(low), ub(upp) {}; 
    double val, lb, ub; 
    void print(FILE* f) const;
  };

  struct _OStr : public _O { 
    _OStr(std::string v, const std::vector<std::string>& range_, const char* description) 
      : _O(description), val(v), range(range_) {};
    std::string val;
    std::vector<std::string> range;
    void print(FILE* f) const;
  };

  std::map<std::string, _O*> mOptions;

  hiopLogger* log;
};


} // ~namespace
#endif 
