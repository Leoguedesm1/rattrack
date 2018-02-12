#ifndef DIRECTORYCREATOR
#define DIRECTORYCREATOR

#include <QDir>

using namespace std;

class DirectoryCreator {

public:
    void create(string path) {

        QDir dir(QString::fromStdString(path));
        if(!dir.exists())
            QDir().mkdir(dir.absolutePath());
    }

    bool removeDir(string path) {
        bool result = true;
        QDir dir(QString::fromStdString(path));

        if(dir.exists()) {
            Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
                if(info.isDir())
                    result = removeDir(info.absoluteFilePath().toUtf8().constData());
                else
                    result = QFile::remove(info.absoluteFilePath());

                if(!result)
                    return result;
            }

            result = QDir().rmdir(QString::fromStdString(path));
        }

        return result;
    }
};

#endif // DIRECTORYCREATOR

