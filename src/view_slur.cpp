/////////////////////////////////////////////////////////////////////////////
// Name:        view_slur.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <sstream>

//----------------------------------------------------------------------------

#include "bboxdevicecontext.h"
#include "comparison.h"
#include "devicecontext.h"
#include "doc.h"
#include "ftrem.h"
#include "functorparams.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "note.h"
#include "options.h"
#include "slur.h"
#include "staff.h"
#include "system.h"
#include "timeinterface.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - Slur
//----------------------------------------------------------------------------

void View::DrawSlur(DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(slur);
    assert(staff);

    FloatingCurvePositioner *curve = this->CalcInitialSlur(dc, slur, x1, x2, staff, spanningType);
    Point points[4];
    curve->GetPoints(points);

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(slur, "", slur->GetUuid(), false);

    int penStyle = AxSOLID;
    switch (slur->GetLform()) {
        case LINEFORM_dashed: penStyle = AxSHORT_DASH; break;
        case LINEFORM_dotted: penStyle = AxDOT; break;
        case LINEFORM_wavy:
        // TODO: Implement wavy slur.
        default: break;
    }
    const int penWidth
        = m_doc->GetOptions()->m_slurEndpointThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const double thicknessCoefficient
        = BoundingBox::GetBezierThicknessCoefficient(points, curve->GetThickness(), penWidth);
    this->DrawThickBezierCurve(
        dc, points, thicknessCoefficient * curve->GetThickness(), staff->m_drawingStaffSize, penWidth, penStyle);

    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(slur, this);
}

FloatingCurvePositioner *View::CalcInitialSlur(
    DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType)
{
    FloatingPositioner *positioner = slur->GetCurrentFloatingPositioner();
    assert(positioner && positioner->Is(FLOATING_CURVE_POSITIONER));
    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);

    if ((this->GetSlurHandling() == SlurHandling::Initialize) && dc->Is(BBOX_DEVICE_CONTEXT)
        && (curve->GetDir() == curvature_CURVEDIR_NONE || curve->IsCrossStaff())) {
        // Initial curve calculation
        slur->SetCachedDrawingX12(x1, x2);
        slur->CalcInitialCurve(m_doc, curve);

        // Update x1 and x2
        Point points[4];
        curve->GetPoints(points);
        x1 = points[0].x;
        x2 = points[3].x;

        // Register content
        const SpannedElements spannedElements = slur->CollectSpannedElements(staff, x1, x2);
        slur->AddSpannedElements(curve, spannedElements, staff, x1, x2);
        slur->AddPositionerToArticulations(curve);
    }
    return curve;
}

} // namespace vrv
