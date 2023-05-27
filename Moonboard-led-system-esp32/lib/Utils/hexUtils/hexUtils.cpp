#include "hexUtils.h"

namespace hexUtils{
    bool uint8ToHex(char *dest, size_t dest_len, const uint8_t *src, size_t src_len)
  {
    if (dest_len < (src_len * 2 + 1)) /* check that dest is large enough */
      return false;
    *dest = '\0'; /* in case val_len==0 */
    while (src_len--)
    {
      /* sprintf directly to where dest points */
      sprintf(dest, "%02X", *src);
      dest += 2;
      ++src;
    }
    return true;
  }

  size_t hexToUint8(uint8_t *dest, size_t dest_len, const char *src)
  {
    size_t i;
    int value;
    for (i = 0; i < dest_len && sscanf(src + i * 2, "%2x", &value) == 1; i++)
    {
      dest[i] = value;
    }
    return i;
  }

  void printHex(const char *label, uint8_t *data, int len)
  {
    Serial.printf("%s:\n", label);

    for (int i = 0; i < len; i++)
    {
      Serial.printf("%02X ", data[i]);
    }
    Serial.printf("\n\n");
  }
}