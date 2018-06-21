//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptAPI.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/AngelScript/ScriptInstance.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <AngelScript/angelscript.h>

#include "InGameEditor.h"

#include <Urho3D/DebugNew.h>
//=============================================================================
//=============================================================================
static InGameEditor *gInGameEditor = NULL;

//=============================================================================
//=============================================================================
InGameEditor::InGameEditor(Context* context)
    : Component(context)
    , scriptFileName_()
    , uiHidden_(false)
{
    gInGameEditor = this;
}

InGameEditor::~InGameEditor()
{
}

// static fns.
void InGameEditor::RegisterObject(Context* context)
{
    context->RegisterFactory<InGameEditor>();
}

void InGameEditor::GameHideUI(bool hide, Node *cameraNode)
{
    gInGameEditor->SetUIHidden(hide, cameraNode);
}

void InGameEditor::SetupSceneAndScript(Scene *scene)
{
    // Instantiate and register the AngelScript subsystem
    Script *script;
    context_->RegisterSubsystem((script = new Script(context_)));
    asIScriptEngine* scriptEngine = script->GetScriptEngine();

    // set script scene
    script->SetDefaultScene(scene);

    // callback fn for when the editor changes UI visiblity
    scriptEngine->RegisterGlobalFunction("void GameHideUI(bool, Node@+)", asFUNCTION(InGameEditor::GameHideUI), asCALL_CDECL);
}

bool InGameEditor::LoadScript(const String &scriptFileName)
{
    scriptFileName_ = scriptFileName;

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    scriptFile_ = cache->GetResource<ScriptFile>(scriptFileName_);

    //**note**
    // if scriptFile_ is NULL then most likely the files in Data/InGameData are not in sync with files in Data/Scripts

    return (scriptFile_ != NULL);
}

bool InGameEditor::RunScript()
{
    bool valid = false;

    if (scriptFile_ && scriptFile_->Execute("void Start()"))
    {
        // Subscribe to script's reload event to allow live-reload of the application
        SubscribeToEvent(scriptFile_, E_RELOADSTARTED, URHO3D_HANDLER(InGameEditor, HandleScriptReloadStarted));
        SubscribeToEvent(scriptFile_, E_RELOADFINISHED, URHO3D_HANDLER(InGameEditor, HandleScriptReloadFinished));
        SubscribeToEvent(scriptFile_, E_RELOADFAILED, URHO3D_HANDLER(InGameEditor, HandleScriptReloadFailed));

        valid = true;
    }

    return valid;
}

void InGameEditor::Stop()
{
    if (scriptFile_)
    {
        // Execute the optional stop function
        if (scriptFile_->GetFunction("void Stop()"))
            scriptFile_->Execute("void Stop()");
    }
}

void InGameEditor::HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData)
{
    if (scriptFile_->GetFunction("void Stop()"))
        scriptFile_->Execute("void Stop()");
}

void InGameEditor::HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData)
{
    // Restart the script application after reload
    if (!scriptFile_->Execute("void Start()"))
    {
        scriptFile_.Reset();
    }
}

void InGameEditor::HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData)
{
    scriptFile_.Reset();
}

