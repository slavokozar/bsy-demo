#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "travel.cc"

class TravelInstance : public pp::Instance {
public:
    explicit TravelInstance(PP_Instance instance)
            : pp::Instance(instance) {}

    virtual ~TravelInstance() {}

    virtual void HandleMessage(const pp::Var &var_message) {
        // Ignore the message if it is not a string.
        if (!var_message.is_string())
            return;

        std::string message = var_message.AsString();

        Graph graph;
        graph.Load(message);
        
        std::string output = std::to_string(graph.Process());

        pp::Var var_reply(output);
        PostMessage(var_reply);
    }
};

class TravelModule : public pp::Module {
public:
    TravelModule() : pp::Module() {}

    virtual ~TravelModule() {}

    virtual pp::Instance *CreateInstance(PP_Instance instance) {
        return new TravelInstance(instance);
    }
};

namespace pp {

    Module *CreateModule() {
        return new TravelModule();
    }

}  // namespace pp
