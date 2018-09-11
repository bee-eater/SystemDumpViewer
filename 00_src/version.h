#ifndef VERSION_H
#define VERSION_H

#define MY_APP_VERSION 2,1,10,22169
#define MY_APP_VERSION_STRING "2.1.10.22169"

/**
 * @brief The Version namespace contains the version information of the application.
 *
 * To rebuild this information you have to open BR.SystemdumpViewer.pro and uncomment this line 
 * #versiontarget.commands = ../BR.SystemdumpViewer/version.exe $$MAJOR $$MINOR $$VERSION_HEADER 
 * build the project once and then place the comment again.
 * Then you manually have to enter the SVN revision since the automatic detection is not yet implemented!
 * \todo Implement automatic SVN revision detection 
 */
namespace Version
{
	static const int MAJOR = 2;		/**< Major version number */
	static const int MINOR = 1;		/**< Minor version number */
    static const int REVISION = 10;	/**< SVN revision */
	static const int BUILD = 22169;	/**< Number depending on day of build */
}

#endif // VERSION_H
