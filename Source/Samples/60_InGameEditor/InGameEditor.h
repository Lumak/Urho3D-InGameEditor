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

#pragma once

#include <Urho3D/Scene/Component.h>

namespace Urho3D
{
class Scene;
class ScriptFile;
}

using namespace Urho3D;

//=============================================================================
//=============================================================================
class InGameEditor : public Component
{
    URHO3D_OBJECT(InGameEditor, Component)

public:
    InGameEditor(Context* context);
    ~InGameEditor();

    static void RegisterObject(Context* context);
    static void GameHideUI(bool hidden, Node *cameraNode);

    void SetupSceneAndScript(Scene *scene);
    bool LoadScript(const String &scriptFileName);
    bool RunScript();
    void Stop();

    void SetUIHidden(bool hidden, Node *cameraNode)
    {
        uiHidden_ = hidden;
        cameraNode_ = cameraNode;
    }

    bool IsUIHidden() const { return uiHidden_; }
    Node *GetSceneCameraNode() const { return cameraNode_; }

protected:
    /// Handle reload start of the script file.
    void HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData);
    /// Handle reload success of the script file.
    void HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData);
    /// Handle reload failure of the script file.
    void HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData);
    /// Parse script file name from the first argument.
    void GetScriptFileName();

protected:
    /// Script file name.
    String scriptFileName_;
    SharedPtr<ScriptFile> scriptFile_;

    // editor
    bool uiHidden_;
    WeakPtr<Node> cameraNode_;

};
