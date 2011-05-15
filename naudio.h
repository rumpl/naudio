#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <v8.h>
#include <node.h>

#include <vlc/vlc.h>

using namespace v8;
using namespace node;

class Naudio : public node::ObjectWrap
{
public:
    static Persistent<FunctionTemplate> s_ct;

    static void Init(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static void WaitPlaying(libvlc_media_player_t *mp);
    static void WaitEnd(libvlc_media_player_t *mp);
    static Handle<Value> Play(const Arguments& args);

    static int EIO_PlayAfter(eio_req *req);
    static int EIO_Play(eio_req *req);
};
