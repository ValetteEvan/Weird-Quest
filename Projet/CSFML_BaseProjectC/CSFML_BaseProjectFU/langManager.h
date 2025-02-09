/// Flexible and modular language API.
/// Function prefix: lang_

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "dirent.h"

#define PATH_LANGS "..\\Ressources\\Languages\\"
#define LANG_LEN_KEY 32
#define LANG_LEN_VAL 512

/// Data holder for individual translation keys
/// A language key is parsed from a JSON file according to the following pattern: "key": "value"
/// Note that "id" and "language_name" are reserved keys and should always be present in a language file.
typedef struct LangKey {
	struct LangKey* prev; /// Linked list architecture data
	struct LangKey* next; /// Linked list architecture data
	char key[32]; /// Translation key ID
	char* value; /// Translation key contents
} LangKey;

/// Data holder for language files
/// Language files are automatically parsed from the assets/lang folder.
/// A language node also serves as the root of a linked list of LangKeys.
typedef struct LangLanguage {
	struct LangLanguage* prev; /// Linked list architecture data
	struct LangLanguage* next; /// Linked list architecture data
	int id; /// Numerical language ID
	char name[32]; /// Language name
	LangKey* keySentinel; /// Sentinel node for the language's translation key list
} LangLanguage;
LangLanguage* lang_LSentinel; /// Sentinel node for the language list

int lang_CurrentID; /// Currently selected language

/// Initializes the language manager and loads all detected language files into memory.
void lang_Init();

/// Parses a language file's JSON structure and loads it into a LangLanguage node.
/// \param _name - Name of the language file to load. Pay attention not to include the folder path.
void lang_LoadFile(char* _name);

/// Adds a language data node to the list.
void lang_AddLang(LangLanguage* _l);

/// Adds a translation key node to a language node's key list.
void lang_AddKey(LangLanguage* _l, LangKey* _k);

/// \param _key - Translation key ID to fetch
/// \return Value corresponding to the given key in the currently selected language
char* lang_GetFromKey(char* _key);

/// Sets the currently selected language.
void lang_SetLang(int _id);

/// Gets the name of the language whose ID is passed as argument.
char* lang_GetName(int _id);

/// \return How many languages are currently loaded into memory
int lang_GetCount();

/// \return ID of the currently selected language
int lang_GetCurrentID();

/// Unloads a language from memory and destroys all associated translation key data.
/// \deprecated Technical only, do not use.
LangLanguage* lang_DestroyL(LangLanguage* _l);

/// Destroys a translation key node.
/// \deprecated Technical only, do not use.
LangKey* lang_DestroyK(LangKey* _k);

/// Clears the language buffer and destroys all associated data.
void lang_ClearBuffer();

void lang_UnitTest();