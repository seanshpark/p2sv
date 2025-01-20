#ifndef __P2SV_PROC_H__
#define __P2SV_PROC_H__

namespace p2sv
{

class Proc
{
public:
  void init(void);
  void final(void);

  float window(int sample) { return _window[sample]; }

private:
  float *_window = nullptr;
};

} // namespace p2sv

#endif // __P2SV_PROC_H__
