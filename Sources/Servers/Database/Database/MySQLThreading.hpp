#ifndef PROJECTWAAAGH_MYSQLTHREADING_HPP
#define PROJECTWAAAGH_MYSQLTHREADING_HPP

#include "Defines.hpp"

namespace MySQL {
    
    PRW_DATABASE_API void Library_Init();
    PRW_DATABASE_API void Library_End();
    PRW_DATABASE_API uint32 GetLibraryVersion();
} // namespace MySQL


#endif //PROJECTWAAAGH_MYSQLTHREADING_HPP