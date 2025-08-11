#ifndef PROTECT_H
#define PROTECT_H

namespace Protection {

bool enableAntiDebug();
bool enableAntiVM();
bool enableMemoryProtection();
bool checkIntegrity();

} // namespace Protection

#endif // PROTECT_H