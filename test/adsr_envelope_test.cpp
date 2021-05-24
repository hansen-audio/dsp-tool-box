// Copyright René Hansen 2016.

#include "ha/dsp_tool_box/modulation/adsr_envelope.h"
#include "gtest/gtest.h"
#include <fstream>

using namespace ha::dtb;
using namespace ha::dtb::modulation;

using TimeValue  = std::pair<real, real>;
using TimeValues = std::vector<TimeValue>;

using TestData = struct
{
    real att;
    real dec;
    real sus;
    real rel;

    const TimeValues kTriggered;
    const TimeValues kReleased;
};

static const TestData kOneFullCycleData = {
    1.f,
    2.f,
    0.5f,
    2.f,
    {
        {0.f, 0.f},        {0.1f, 0.1f},      {0.2f, 0.2f},
        {0.3f, 0.3f},      {0.4f, 0.4f},      {0.5f, 0.5f},
        {0.6f, 0.6f},      {0.7f, 0.7f},      {0.8f, 0.8f},
        {0.9f, 0.9f},      {1.f, 1.f},        {1.1f, 0.78772f},
        {1.2f, 0.665565f}, {1.3f, 0.595273f}, {1.4f, 0.554824f},
        {1.5f, 0.531548f}, {1.6f, 0.518154f}, {1.7f, 0.510446f},
        {1.8f, 0.506011f}, {1.9f, 0.503459f}, {2.f, 0.501991f},
        {2.1f, 0.501145f}, {2.2f, 0.500659f}, {2.3f, 0.500379f},
        {2.4f, 0.500218f}, {2.5f, 0.500126f}, {2.6f, 0.500072f},
        {2.7f, 0.500042f}, {2.8f, 0.500024f}, {2.9f, 0.500014f},
        {3.f, 0.500008f},  {3.1f, 0.5f},      {3.2f, 0.5f},
        {3.3f, 0.5f},      {3.4f, 0.5f},      {3.5f, 0.5f},
        {3.6f, 0.5f},      {3.7f, 0.5f},      {3.8f, 0.5f},
        {3.9f, 0.5f},      {4.f, 0.5f},
    },
    {
        {0.f, 0.5f},          {0.1f, 0.28772f},     {0.2f, 0.165566f},
        {0.3f, 0.095273f},    {0.4f, 0.0548239f},   {0.5f, 0.0315478f},
        {0.6f, 0.0181539f},   {0.7f, 0.0104465f},   {0.8f, 0.00601131f},
        {0.9f, 0.00345916f},  {1.f, 0.00199053f},   {1.1f, 0.00114542f},
        {1.2f, 0.000659138f}, {1.3f, 0.000379294f}, {1.4f, 0.000218272f},
        {1.5f, 0.000125587f}, {1.6f, 7.22706e-05f}, {1.7f, 4.15742e-05f},
        {1.8f, 2.39313e-05f}, {1.9f, 1.37687e-05f},
    },
};

//-----------------------------------------------------------------------------
static const TestData kReleaseInAttackData = {
    3.f,
    2.f,
    0.5f,
    3.f,
    {
        {0.f, 0.f},   {0.1f, 0.0333333f}, {0.2f, 0.0666667f},
        {0.3f, 0.1f}, {0.4f, 0.133333f},  {0.5f, 0.166667f},
        {0.6f, 0.2f}, {0.7f, 0.233333f},  {0.8f, 0.266667f},
        {0.9f, 0.3f}, {1.f, 0.333333f},   {1.1f, 0.366667f},
        {1.2f, 0.4f}, {1.3f, 0.433333f},  {1.4f, 0.466667f},
        {1.5f, 0.5f}, {1.6f, 0.533333f},  {1.7f, 0.566667f},
        {1.8f, 0.6f}, {1.9f, 0.633333f},
    },
    {
        {0.f, 0.633333f},     {0.1f, 0.43816f},     {0.2f, 0.303132f},
        {0.3f, 0.209716f},    {0.4f, 0.145088f},    {0.5f, 0.100377f},
        {0.6f, 0.0694436f},   {0.7f, 0.0480433f},   {0.8f, 0.0332378f},
        {0.9f, 0.0229949f},   {1.f, 0.0159086f},    {1.1f, 0.0110061f},
        {1.2f, 0.00761431f},  {1.3f, 0.00526786f},  {1.4f, 0.00364447f},
        {1.5f, 0.00252134f},  {1.6f, 0.00174433f},  {1.7f, 0.00120676f},
        {1.8f, 0.000834882f}, {1.9f, 0.000577629f}, {2.f, 0.00039959f},
        {2.1f, 0.000276506f}, {2.2f, 0.000191271f}, {2.3f, 0.000132322f},
        {2.4f, 9.15527e-05f}, {2.5f, 6.33597e-05f}, {2.6f, 4.38094e-05f},
        {2.7f, 3.03388e-05f}, {2.8f, 2.09808e-05f}, {2.9f, 1.44839e-05f},
        {3.f, 1.00136e-05f},
    },
};
//-----------------------------------------------------------------------------
void recordEnvelopeResponse(real att,
                            real dec,
                            real sus,
                            real susHold,
                            real rel,
                            real timeUntilRelease,
                            real timeDelta,
                            std::string dataName)
{
    adsr_envelope_processor adsr;
    adsr.set_att(att);
    adsr.set_dec(dec);
    adsr.set_sus(sus);
    adsr.set_rel(rel);

    //-----------
    std::ofstream file("D:\\TestData.txt");
    file << "static const TestData " << dataName << " = {" << std::endl;
    file << att << "f, " << dec << "f, " << sus << "f, " << rel << "f, "
         << std::endl;
    //----------- trigger
    adsr.trigger();

    file << "{" << std::endl;
    float time = 0.f;
    while (time < timeUntilRelease)
    {
        file << "{" << time << "f, " << adsr.read(time) << "f}," << std::endl;
        time += timeDelta;
    }
    file << "}, " << std::endl;
    //----------- release
    adsr.release();
    file << "{" << std::endl;
    time = 0.f;
    while (time < rel)
    {
        file << "{" << time << "f, " << adsr.read(time) << "f}," << std::endl;
        time += timeDelta;
    }
    file << "}, " << std::endl;
    //-----------
    file << "};";
}

//-----------------------------------------------------------------------------
void trigger_envelope_test(const TestData& data)
{
    static const float kFloatError = 0.00001f;

    adsr_envelope_processor adsr;
    adsr.set_att(data.att);
    adsr.set_dec(data.dec);
    adsr.set_sus(data.sus);
    adsr.set_rel(data.rel);

    adsr.trigger();
    for (auto& timeValue : data.kTriggered)
    {
        float value = adsr.read(timeValue.first);
        EXPECT_NEAR(timeValue.second, value, kFloatError);
    }

    adsr.release();
    for (auto& timeValue : data.kReleased)
    {
        float value = adsr.read(timeValue.first);
        EXPECT_NEAR(timeValue.second, value, kFloatError);
    }
}

//-----------------------------------------------------------------------------
TEST(ADSRTest, testOneCompleteCycle)
{
    // recordEnvelopeResponse(2.f, 2.f, 0.5f, 1.f, 2.f, 4.f, 0.1f,
    // "kOneFullCycleData");
    trigger_envelope_test(kOneFullCycleData);
}

//-----------------------------------------------------------------------------
TEST(ADSRTest, testReleaseInAttack)
{
    // recordEnvelopeResponse(3.f, 2.f, 0.5f, 1.f, 3.f, 2.f, 0.1f,
    // "kReleaseInAttackData");
    trigger_envelope_test(kReleaseInAttackData);
}
