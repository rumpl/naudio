#include "naudio.h"

using namespace v8;
using namespace node;

struct play_request
{
    char* file;
    Persistent<Function> cb;
};

void Naudio::Init(Handle<Object> target)
{
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("Naudio"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "play", Play);
    target->Set(String::NewSymbol("Naudio"), s_ct->GetFunction());
}

Handle<Value> Naudio::New(const Arguments& args)
{
    HandleScope scope;
    Naudio *naudio = new Naudio();
    naudio->Wrap(args.This());
    return args.This();
}

Handle<Value> Naudio::Play(const Arguments& args)
{
    HandleScope scope;

    if (args.Length() == 0 || !args[0]->IsString())
    {
        return ThrowException(Exception::Error(String::New("First argument must be a string")));
    }

    if (args.Length() == 0 || !args[1]->IsFunction())
    {
        return ThrowException(Exception::Error(String::New("Second argument must be a function")));
    }

    Local<Function> cb = Local<Function>::Cast(args[1]);
    String::Utf8Value input(args[0]->ToString());

    play_request *pr = new play_request();
    pr->file = strdup(*input);
    pr->cb = Persistent<Function>::New(cb);

    eio_custom(EIO_Play, EIO_PRI_DEFAULT, EIO_PlayAfter, pr);
    ev_ref(EV_DEFAULT_UC);

    return Undefined();
}

void Naudio::WaitPlaying(libvlc_media_player_t *mp)
{
    libvlc_state_t state;
    do {
        state = libvlc_media_player_get_state (mp);
        usleep(50000);
    } while(state != libvlc_Playing &&
            state != libvlc_Error &&
            state != libvlc_Ended );
}

void Naudio::WaitEnd(libvlc_media_player_t *mp)
{
    libvlc_state_t state;
    do {
      state = libvlc_media_player_get_state (mp);
        usleep(100000);
    } while(state != libvlc_Error &&
            state != libvlc_Ended );
}

int Naudio::EIO_Play(eio_req *req)
{
    struct play_request * pr = (struct play_request *)req->data;
    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    inst = libvlc_new (0, NULL);

    if(strncmp(pr->file, "http", 4) == 0)
    {
        m = libvlc_media_new_location (inst, pr->file);
    }
    else
    {
        m = libvlc_media_new_path (inst, pr->file);
    }

    mp = libvlc_media_player_new_from_media (m);

    libvlc_media_release (m);

    libvlc_media_player_play (mp);

    Naudio::WaitPlaying(mp);

    Naudio::WaitEnd(mp);

    libvlc_media_player_stop (mp);

    libvlc_media_player_release (mp);

    libvlc_release (inst);

    return 0;
}

int Naudio::EIO_PlayAfter(eio_req *req)
{
    HandleScope scope;
    ev_unref(EV_DEFAULT_UC);
    struct play_request * pr = (struct play_request *)req->data;
    TryCatch try_catch;
    Local<Value> argv[1];
    argv[0] = Local<Value>::New(Null());
    pr->cb->Call(Context::GetCurrent()->Global(), 0, argv);

    if (try_catch.HasCaught())
    {
        FatalException(try_catch);
    }

    pr->cb.Dispose();
    free(pr);

    return 0;
}

Persistent<FunctionTemplate> Naudio::s_ct;

extern "C" void init(Handle<Object> target)
{
    HandleScope scope;

    Naudio::Init(target);
};
