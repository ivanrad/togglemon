// SPDX-License-Identifier: MIT
//
#include <stdlib.h>
#include <CoreGraphics/CoreGraphics.h>

extern CGError CGSGetDisplayList(
  uint32_t /* max_display */,
  CGDirectDisplayID * /* display_ids */,
  uint32_t * /* display_count */);

extern CGError CGSConfigureDisplayEnabled(
  CGDisplayConfigRef /* config */,
  CGDirectDisplayID /* display_id */,
  bool /* enabled */);

static void die(char *msg) {
  fprintf(stderr, "error: %s\n", msg);
  exit(EXIT_FAILURE);
}

int main(void) {
  const uint32_t max_display = 0x20;
  CGDirectDisplayID display_ids[max_display];
  CGDirectDisplayID main_display_id;
  uint32_t display_count;
  CGError err;

  main_display_id = CGMainDisplayID();

  if ((err = CGSGetDisplayList(max_display, display_ids, &display_count)) != kCGErrorSuccess)
    die("CGSGetDisplayList failed");

  fprintf(stdout, "display count: %u\n", display_count);

  if (display_count > max_display)
    display_count = max_display;

  for (int i = 0; i < (int)display_count; ++i) {
    CGDirectDisplayID display_id = display_ids[i];
    bool is_builtin = CGDisplayIsBuiltin(display_id);
    bool is_online = CGDisplayIsOnline(display_id);
    bool is_active = CGDisplayIsActive(display_id);
    bool is_main = display_id == main_display_id;
    int width = CGDisplayPixelsWide(display_id);
    int height = CGDisplayPixelsHigh(display_id);

    fprintf(stdout,
      "display_id: %u, res: %d" "x" "%d, is_builtin: %d, "
      "is_online: %d, is_active: %d, is_main: %d\n",
      display_id, width, height, is_builtin, is_online, is_active, is_main);

    if (is_builtin) {
      CGDisplayConfigRef config;

      fprintf(stdout, "toggling display_id %u...\n", display_id);

      if ((err = CGBeginDisplayConfiguration(&config)) != kCGErrorSuccess)
        die("CGBeginDisplayConfiguration failed");
      if ((err = CGSConfigureDisplayEnabled(config, display_id, !is_active)) != kCGErrorSuccess)
        die("CGSConfigureDisplayEnabled failed");
      if ((err = CGCompleteDisplayConfiguration(config, kCGConfigurePermanently)) != kCGErrorSuccess)
        die("CGCompleteDisplayConfiguration failed");
    }
  }
  exit(EXIT_SUCCESS);
}
