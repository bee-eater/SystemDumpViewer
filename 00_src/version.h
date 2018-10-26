#ifndef VERSION_H
#define VERSION_H

#define MY_APP_VERSION 2,2,16,24297
#define MY_APP_VERSION_STRING "2.2.16.24297"

/**
 * @brief The Version namespace contains the version information of the application.
 *
 * REVISION = Incrementing with BugFixes
 * BUILD = (<currentyear> - 1994)*1000 + <dayofcurrentyear> 
 */
namespace Version
{
	static const int MAJOR = 2;		/**< Major version number */
    static const int MINOR = 2;		/**< Minor version number */
    static const int REVISION = 16;	/**< Git revision */
    static const int BUILD = 24297;	/**< Number depending on day of build */
}

#endif // VERSION_H
