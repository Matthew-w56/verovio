/////////////////////////////////////////////////////////////////////////////
// Name:        keyaccid.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_KEYACCID_H__
#define __VRV_KEYACCID_H__

#include "atts_gestural.h"
#include "layerelement.h"
#include "pitchinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// KeyAccid
//----------------------------------------------------------------------------

/**
 * This class models the MEI <keyAccid> element.
 */
class KeyAccid : public LayerElement,
                 public PitchInterface,
                 public AttAccidental,
                 public AttColor,
                 public AttEnclosingChars {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    KeyAccid();
    virtual ~KeyAccid();
    Object *Clone() const override { return new KeyAccid(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "KeyAccid"; }
    ///@}

    PitchInterface *GetPitchInterface() override { return dynamic_cast<PitchInterface *>(this); }

    /**
     * Retrieve SMuFL string for the accidental.
     * This will include brackets
     */
    std::wstring GetSymbolStr() const;

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
