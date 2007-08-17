// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <t1lib.h>
#include <config.h>
#include <map>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <list>

#include "String.hh"
#include "T1_FontDataBase.hh"
#include "FontDataBase.hh"
#include "AbstractLogger.hh"
#include "Configuration.hh" 

T1_FontDataBase::T1_DataBase::T1_DataBase(String& name, int i, int initVal)
  : fontName(name), id(i)
{
  for (int i = 0; i < 256; i++)
    used[i] = initVal;
}

T1_FontDataBase::T1_FontDataBase(const SmartPtr<AbstractLogger>& l,
				 const SmartPtr<class Configuration>& conf,
				 bool sub)
 : FontDataBase(), logger(l), subset(sub)
{
  int result;
  std::vector<String> s = conf->getStringList("default/t1lib/t1-font-path");
  for (std::vector<String>::iterator p = s.begin(); p != s.end(); p++)
  {
    result = T1_AddToFileSearchPath(T1_PFAB_PATH, T1_APPEND_PATH, const_cast<char*>(p->c_str()));
    if (result != 0)
    {
      logger->out(LOG_ERROR, "could not add FontDataBase: %s", p->c_str());
      exit (-1);
    }
  }

  void* res = T1_InitLib(LOGFILE | IGNORE_FONTDATABASE); 
  if (res == NULL)
  {
    logger->out(LOG_ERROR, "could not initialize t1lib");
    exit (-1);
  }
}

T1_FontDataBase::~T1_FontDataBase()
{  
  int res = T1_CloseLib();
  if (res != 0)
    logger->out(LOG_INFO, 
	        "t1lib could not uninitialize itself properly, please consult the log file");
}

SmartPtr<T1_FontDataBase>
T1_FontDataBase::create(const SmartPtr<AbstractLogger>& l,
                        const SmartPtr<class Configuration>& conf,
			bool subset)
{ return new T1_FontDataBase(l, conf, subset); }
	
int 
T1_FontDataBase::getFontId(const String& fontName, float fontSize)
{
  int n = T1_GetNoFonts();
 
  String fileName = toLowerCase(fontName) + ".pfb";

  int i;
  for (i = 0; i < n && (strcmp(fileName.c_str(), T1_GetFontFileName(i))); i++);

  if (i == n)
  {
    logger->out(LOG_INFO, "adding font file '%s' to the font database", fileName.c_str());
    i = T1_AddFont(const_cast<char*>(fileName.c_str()));
  
    if (i < 0)
    {
      logger->out(LOG_WARNING, "could not load Type1 font file `%s'", fileName.c_str());
      exit (-1);
    }	

    logger->out(LOG_INFO, "loading font ID: %d", i);
    T1_LoadFont(i);

    if (subset) // subset
      T1_dB.push_back(T1_DataBase(fileName, i, 0));
    else // embed
      T1_dB.push_back(T1_DataBase(fileName, i, 1));
  } 
  else
    logger->out(LOG_DEBUG, "font '%s' exists in the fontDataBase", fileName.c_str());
   
  int id = FontDataBase::getFontId(fontName, fontSize);
  return (id);
}

void 
T1_FontDataBase::dumpFontTable(std::ostream& os) const
{
  os << "%%DocumentSuppliedResources: font" << std::endl;

  for (std::list<T1_DataBase>::const_iterator p = T1_dB.begin(); p != T1_dB.end(); p++)
    os << "%%+ font " << p->fontName << std::endl;
  os << std::endl;
  
  os << "%%BeginSetup" << std::endl;

  for (std::list<T1_DataBase>::const_iterator p = T1_dB.begin(); p != T1_dB.end(); p++)
  {
    logger->out(LOG_DEBUG, "subset font `%s'", (p->fontName).c_str());
    unsigned int count = 0;
    for (unsigned int i = 0; i < 256; i++)
      if (p->used[i]) count++;
    logger->out(LOG_DEBUG, "subsetting %d chars", count);

    const char* use = (char*)(p->used);

    unsigned long bufSize;
    char* dump = T1_SubsetFont(p -> id, const_cast<char*>(use),
		               T1_SUBSET_DEFAULT | T1_SUBSET_SKIP_REENCODE,
		               64, 16384, &bufSize);
    os << "%%BeginResource: font " << p->fontName << std::endl;
    os.write(dump, bufSize);
    os << "%%EndResource" << std::endl;
    logger->out(LOG_DEBUG, "done!");
    
    free(dump);
  } 
  
  os << "%%EndSetup" << std::endl << std::endl;

  FontDataBase::dumpFontTable(os);   
}

void
T1_FontDataBase::recallFont(const int id, std::ostringstream& body)
{
  FontDataBase::recallFont(id, body);
}

void
T1_FontDataBase::usedChar(const String& content, const String& family)
{
  if (subset)
  {
    String fileName = toLowerCase(family) + ".pfb";
    std::list<T1_DataBase>::iterator it;
   
    for (it = T1_dB.begin(); (it->fontName) != fileName; it++);

    for (String::const_iterator i = content.begin(); i != content.end(); i++)
      it->used[(unsigned int)(*i)] = 1;
  }
}

