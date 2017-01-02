#ifndef ERRORSHANDLER_HPP
#define ERRORSHANDLER_HPP

#define USE_QGLVIEWER

#include <QtGlobal>
#include <random>

namespace Random {
static std::random_device rd;
static std::mt19937 genMt19937(rd());
}

#define ERROR_UNKNOWN do {qFatal("An unknown critical error occured.");} while(0)
#define ERROR(message) do {qFatal(message);} while(0)

#define WARNING_UNKNOWN do {qWarning("An unknown error occured.");} while(0)
#define WARNING(message) do {qWarning(message);} while(0)

//defining a tolerance to floating point errors
#define EPSILON 1e-5f

#endif // ERRORSHANDLER_HPP
