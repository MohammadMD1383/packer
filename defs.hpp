#ifndef PACKER_MANIFEST_HPP
#define PACKER_MANIFEST_HPP

#define VERSION "0.1.0"
#define BUFFER_SIZE (1 * 1024 * 1024 * 100)

#define VERBOSE if (verbose)
#define INFO(...)    cout << "\033[1;46;97m[INFO]\033[0;36m "     << __VA_ARGS__ << "\033[0m" << endl
#define WARNING(...) cout << "\033[1;43;97m[WARNING]\033[0;33m "  << __VA_ARGS__ << "\033[0m" << endl
#define ERROR(...)   cout << "\033[1;101;97m[ERROR]\033[0;91m "   << __VA_ARGS__ << "\033[0m" << endl

#define loop while (true)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define WINDOWS
#endif

#endif //PACKER_MANIFEST_HPP
