#pragma once

#ifdef YO_MAMA_SO_FAT_DEBUG_LOGS
#define DebugLog() if constexpr(true)
#define DebugLogIf(x) if constexpr(!!x)
#else
#define DebugLog() if constexpr(false)
#define DebugLogIf(x) if constexpr(false)
#endif
