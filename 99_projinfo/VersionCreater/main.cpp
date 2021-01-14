#include <iostream>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QFileInfo>
#include <QTemporaryFile>
#include <cstdlib>

using namespace std;

static int getBuildNumber()
{
  const QDate today(QDate::currentDate());
  return ((today.year() - 1994) * 1000) + today.dayOfYear();
}

static QByteArray readFile(const QString& fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
  {
    return QByteArray();
  }
  return file.readAll();
}

static int writeFile(const QString& fileName, const int major, const int minor, const int bugfix, const int build)
{
  // Create a temp file containing the version info and
  // only replace the existing one if they are different
  QTemporaryFile tempFile;
  if (tempFile.open())
  {
    QTextStream out(&tempFile);
    out << "#ifndef VERSION_H\r\n";
    out << "#define VERSION_H\r\n\r\n";
    out << "#define MY_APP_VERSION " << major << "," << minor << "," << bugfix << "," << build << "\r\n";
    out << "#define MY_APP_VERSION_STRING " << "\""<< major << "." << minor << "." << bugfix << "." << build << "\"\r\n\r\n";
    out << "/**\n * @brief The Version namespace contains the version information of the application.\n *\n * To rebuild this information you have to open BR.SystemdumpViewer.pro and uncomment this line \n * #versiontarget.commands = ../github/00_src/version.exe $$MAJOR $$MINOR $$BUGFIX $$VERSION_HEADER \n * build the project once and then place the comment again.\n */\n";
    out << "namespace Version\r\n";
    out << "{\r\n";
    out << "\tstatic const int MAJOR = " << major << ";\t\t/**< Major version number */\r\n";
    out << "\tstatic const int MINOR = " << minor << ";\t\t/**< Minor version number */\r\n";
    out << "\tstatic const int REVISION = " << bugfix << ";\t/**< Bugfix version number */\r\n";
    out << "\tstatic const int BUILD = " << build << ";\t/**< Number depending on day of build */\r\n";
    out << "}\r\n\r\n";
    out << "#endif // VERSION_H\r\n";

    const QString tempFileName = tempFile.fileName();
    tempFile.close();

    if (!QFile::exists(fileName) || readFile(fileName) != readFile(tempFileName))
    {
      QFile::remove(fileName);
      QFile::copy(tempFileName, fileName);
    }

    return 0;
  }
  else
  {
    cout << "Error creating temporary file!" << endl;
    return 1;
  }
}

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    cout << "Usage: major minor bugfix filename" << endl;
    return 1;
  }

  const int major = atoi(argv[1]);
  const int minor = atoi(argv[2]);
  const int bugfix = atoi(argv[3]);
  const int build = getBuildNumber();

  cout << major << '.' << minor << '.' << bugfix << '.' << build << endl;

  return writeFile(argv[4], major, minor, bugfix, build);
}
