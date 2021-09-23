#ifndef NVS_LOADER_H_
#define NVS_LOADER_H_

#include <FS.h>
#include <Preferences.h>
#include <SPIFFS.h>

class NVSLoader {
public:
    NVSLoader(const char* nvsNamespace);
    virtual ~NVSLoader(void);

    virtual bool begin(const char* path);
    virtual bool get(const char* key, char* buf, size_t size);
    virtual const char* const getNamespace(void) const;

protected:
    virtual bool load(const char* path);
    virtual bool parse(const char* buf, size_t size);
    virtual bool readFile(const char* path, char* buf, size_t size);
    virtual bool removeFile(const char* path);

private:
    static const bool FORMAT_SPIFFS_IF_FAILED = true;
    static const size_t NVS_NAMESPACE_LEN = 15;

    Preferences prefs;
    char NAMESPACE[NVS_NAMESPACE_LEN + 1];
};

#endif