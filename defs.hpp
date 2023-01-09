#ifndef PACKER_MANIFEST_HPP
#define PACKER_MANIFEST_HPP

#define VERSION "0.1.0"
#define BUFFER_SIZE (1 * 1024 * 1024 * 100)

#define VERBOSE if (verbose)

#ifndef WINDOWS
#define INFO_MSG(...)    cout << "\033[1;46;97m[INFO_MSG]\033[0;36m "    << __VA_ARGS__ << "\033[0m" << endl
#define WARNING_MSG(...) cout << "\033[1;43;97m[WARNING_MSG]\033[0;33m " << __VA_ARGS__ << "\033[0m" << endl
#define ERROR_MSG(...)   cout << "\033[1;101;97m[ERROR_MSG]\033[0;91m "  << __VA_ARGS__ << "\033[0m" << endl
#else
#define INFO_MSG(...)    cout << windowsInfo()    << __VA_ARGS__ << windowsNormal() << endl
#define WARNING_MSG(...) cout << windowsWarning() << __VA_ARGS__ << windowsNormal() << endl
#define ERROR_MSG(...)   cout << windowsError()   << __VA_ARGS__ << windowsNormal() << endl
#endif

#define loop while (true)

#endif //PACKER_MANIFEST_HPP
