#ifndef VERSION_H
#define VERSION_H

#define MY_APP_VERSION 2,4,1,27183
#define MY_APP_VERSION_STRING "2.4.1.27183"

/**
 * @brief The Version namespace contains the version information of the application.
 *
 * To rebuild this information you have to open BR.SystemdumpViewer.pro and uncomment this line 
 * #versiontarget.commands = ../github/00_src/version.exe $$MAJOR $$MINOR $$BUGFIX $$VERSION_HEADER 
 * build the project once and then place the comment again.
 */
namespace Version
{
	static const int MAJOR = 2;		/**< Major version number */
	static const int MINOR = 4;		/**< Minor version number */
	static const int REVISION = 1;	/**< Bugfix version number */
	static const int BUILD = 27183;	/**< Number depending on day of build */
}

#endif // VERSION_H
