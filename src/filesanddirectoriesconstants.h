#ifndef FILESANDDIRECTORIESCONSTANTS
#define FILESANDDIRECTORIESCONSTANTS

#include <QDir>

using namespace std;

//Directories Names
const string CALIBRATION_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Calibration";
const string TESTES_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Testes";
const string SNAPSHOT_DIR_NAME = "/Snapshots";
const string SETTINGS_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Settings";

//Analysis files names
const string STATISTICS_FILE_NAME = "/statistics.csv";
const string LEARNING_FILE_NAME = "/learning.csv";

//Cam configuration files names
const string INFO_FILE_NAME = "/info.yml";
const string CALIB_FILE_NAME = "/calibration.yml";
const string HOMOGRAPHY_FILE_NAME = "/homography.yml";
const string QUADRANT_FILE_NAME = "/quadrants.yml";

//Advanced Settings files names
const string ADVANCED_SETTINGS_FILE_NAME = "/advSettings.yml";

#endif // FILESANDDIRECTORIESCONSTANTS

