#include "settings.h"
#include <defaults.h>

using namespace displace::vesselsEditor;

const QString Settings::KeyConfigScriptPath = "R::GenerateVesselsConfigFiles.R";
const QString Settings::KeyRunScriptPath = "R::RunVesselsConfigFiles.R";

QString Settings::getConfigScriptPath()
{
    return settings.value(KeyConfigScriptPath,
                          displace::vesselsEditor::getDefaultConfigScriptPath()).toString();
}

void Settings::setConfigScriptPath(const QString &path)
{
    settings.setValue(KeyConfigScriptPath, path);
}

QString Settings::getRunScriptPath()
{
    return settings.value(KeyRunScriptPath,
                          displace::vesselsEditor::getDefaultRunScriptPath()).toString();
}

void Settings::setRunScriptPath(const QString &path)
{
    settings.setValue(KeyRunScriptPath, path);
}

