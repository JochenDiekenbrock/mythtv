/*
 * \file dvbdevtree_cfg.cpp
 * \brief DVB-S Device Tree Configuration Classes.
 * \author Copyright (C) 2006, Yeasah Pell
 */

// Std C headers
#include <cmath>

// MythTV headers
#include "mythdbcon.h"
#include "mythlogging.h"
#include "diseqcsettings.h"

/* Lat/Long items relocated from videosource.cpp */

static GlobalLineEdit *DiSEqCLatitude(void)
{
    GlobalLineEdit *gc = new GlobalLineEdit("latitude");
    gc->setLabel("Latitude");
    gc->setHelpText(
        DeviceTree::tr("The Cartesian latitude for your location. "
                       "Use negative numbers for southern coordinates."));
    return gc;
}

static GlobalLineEdit *DiSEqCLongitude(void)
{
    GlobalLineEdit *gc = new GlobalLineEdit("longitude");
    gc->setLabel("Longitude");
    gc->setHelpText(
        DeviceTree::tr("The Cartesian longitude for your location. "
                       "Use negative numbers for western coordinates."));
    return gc;
}

//////////////////////////////////////// DeviceTypeSetting

class DeviceTypeSetting : public ComboBoxSetting, public Storage
{
  public:
    DeviceTypeSetting(DiSEqCDevDevice &device) :
        ComboBoxSetting(this), m_device(device)
    {
        setLabel(DeviceTree::tr("Device Type"));
        addSelection(DeviceTree::tr("Switch"),
                     QString::number((uint) DiSEqCDevDevice::kTypeSwitch));
        addSelection(DeviceTree::tr("Rotor"),
                     QString::number((uint) DiSEqCDevDevice::kTypeRotor));
        addSelection(DeviceTree::tr("Unicable"),
                     QString::number((uint) DiSEqCDevDevice::kTypeSCR));
        addSelection(DeviceTree::tr("LNB"),
                     QString::number((uint) DiSEqCDevDevice::kTypeLNB));
    }

    virtual void Load(void)
    {
        QString tmp = QString::number((uint) m_device.GetDeviceType());
        setValue(getValueIndex(tmp));
    }

    virtual void Save(void)
    {
        m_device.SetDeviceType(
            (DiSEqCDevDevice::dvbdev_t) getValue().toUInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevDevice &m_device;
};

//////////////////////////////////////// DeviceDescrSetting

class DeviceDescrSetting : public LineEditSetting, public Storage
{
  public:
    DeviceDescrSetting(DiSEqCDevDevice &device) :
        LineEditSetting(this), m_device(device)
    {
        setLabel(DeviceTree::tr("Description"));
        QString help = DeviceTree::tr(
            "Optional descriptive name for this device, to "
            "make it easier to configure settings later.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(m_device.GetDescription());
    }

    virtual void Save(void)
    {
        m_device.SetDescription(getValue());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevDevice &m_device;
};


//////////////////////////////////////// DeviceRepeatSetting

class DeviceRepeatSetting : public SpinBoxSetting, public Storage
{
  public:
    DeviceRepeatSetting(DiSEqCDevDevice &device) :
        SpinBoxSetting(this, 0, 15, 1), m_device(device)
    {
        setLabel(DeviceTree::tr("Repeat Count"));
        QString help = DeviceTree::tr(
            "Number of repeat (command with repeat flag ON) or resend (the same command) DiSEqC commands. "
            "If value is higher than 10, command will be resend N-10 times. "
            "If value is lower than 10, command will be repeated N times. "
            "Repeat useful for unreliable DiSEqC equipment; resend useful when unreliable DiSEqC equipment has broken/unsupported repeat flag support.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(m_device.GetRepeatCount());
    }

    virtual void Save(void)
    {
        m_device.SetRepeatCount(getValue().toUInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevDevice &m_device;
};

//////////////////////////////////////// SwitchTypeSetting

class SwitchTypeSetting : public ComboBoxSetting, public Storage
{
  public:
    SwitchTypeSetting(DiSEqCDevSwitch &switch_dev) :
        ComboBoxSetting(this), m_switch(switch_dev)
    {
        setLabel(DeviceTree::tr("Switch Type"));
        setHelpText(DeviceTree::tr("Select the type of switch from the list."));

        addSelection(DeviceTree::tr("Tone"),
                     QString::number((uint) DiSEqCDevSwitch::kTypeTone));
        addSelection(DeviceTree::tr("Voltage"),
                     QString::number((uint) DiSEqCDevSwitch::kTypeVoltage));
        addSelection(DeviceTree::tr("Mini DiSEqC"),
                     QString::number((uint) DiSEqCDevSwitch::kTypeMiniDiSEqC));
        addSelection(DeviceTree::tr("DiSEqC"),
                     QString::number((uint)
                                     DiSEqCDevSwitch::kTypeDiSEqCCommitted));
        addSelection(DeviceTree::tr("DiSEqC (Uncommitted)"),
                     QString::number((uint)
                                     DiSEqCDevSwitch::kTypeDiSEqCUncommitted));
        addSelection(DeviceTree::tr("Legacy SW21"),
                     QString::number((uint) DiSEqCDevSwitch::kTypeLegacySW21));
        addSelection(DeviceTree::tr("Legacy SW42"),
                     QString::number((uint) DiSEqCDevSwitch::kTypeLegacySW42));
        addSelection(DeviceTree::tr("Legacy SW64"),
                     QString::number((uint) DiSEqCDevSwitch::kTypeLegacySW64));
    }

    virtual void Load(void)
    {
        setValue(getValueIndex(QString::number((uint) m_switch.GetType())));
    }

    virtual void Save(void)
    {
        m_switch.SetType((DiSEqCDevSwitch::dvbdev_switch_t)
                         getValue().toUInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevSwitch &m_switch;
};

//////////////////////////////////////// SwitchAddressSetting

class SwitchAddressSetting : public LineEditSetting, public Storage
{
  public:
    SwitchAddressSetting(DiSEqCDevSwitch &switch_dev) :
           LineEditSetting(this), m_switch(switch_dev)
    {
        setLabel(DeviceTree::tr("Address of switch"));
        setHelpText(DeviceTree::tr("The DiSEqC address of the switch."));
    }

    virtual void Load(void)
    {
        setValue(QString("0x%1").arg(m_switch.GetAddress(), 0, 16));
    }

    virtual void Save(void)
    {
        m_switch.SetAddress(getValue().toUInt(0, 16));
    }
    virtual void Save(QString /*destination*/) { Save(); }

  private:
    DiSEqCDevSwitch &m_switch;
};

//////////////////////////////////////// SwitchPortsSetting

class SwitchPortsSetting : public LineEditSetting, public Storage
{
  public:
    SwitchPortsSetting(DiSEqCDevSwitch &switch_dev) :
        LineEditSetting(this), m_switch(switch_dev)
    {
        setLabel(DeviceTree::tr("Number of ports"));
        setHelpText(DeviceTree::tr("The number of ports this switch has."));
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_switch.GetNumPorts()));
    }

    virtual void Save(void)
    {
        m_switch.SetNumPorts(getValue().toUInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevSwitch &m_switch;
};

//////////////////////////////////////// SwitchConfig

SwitchConfig::SwitchConfig(DiSEqCDevSwitch &switch_dev)
{
    ConfigurationGroup *group =
        new VerticalConfigurationGroup(false, false);
    group->setLabel(DeviceTree::tr("Switch Configuration"));

    group->addChild(new DeviceDescrSetting(switch_dev));
    group->addChild(new DeviceRepeatSetting(switch_dev));
    m_type = new SwitchTypeSetting(switch_dev);
    group->addChild(m_type);
    m_address = new SwitchAddressSetting(switch_dev);
    group->addChild(m_address);
    m_ports = new SwitchPortsSetting(switch_dev);
    group->addChild(m_ports);

    connect(m_type, SIGNAL(valueChanged(const QString&)),
            this,   SLOT(  update(void)));

    addChild(group);
}

void SwitchConfig::update(void)
{
    switch ((DiSEqCDevSwitch::dvbdev_switch_t) m_type->getValue().toUInt())
    {
        case DiSEqCDevSwitch::kTypeTone:
        case DiSEqCDevSwitch::kTypeVoltage:
        case DiSEqCDevSwitch::kTypeMiniDiSEqC:
        case DiSEqCDevSwitch::kTypeLegacySW21:
        case DiSEqCDevSwitch::kTypeLegacySW42:
            m_address->setValue(QString("0x10"));
            m_address->setEnabled(false);
            m_ports->setValue("2");
            m_ports->setEnabled(false);
            break;
        case DiSEqCDevSwitch::kTypeLegacySW64:
            m_address->setValue(QString("0x10"));
            m_address->setEnabled(false);
            m_ports->setValue("3");
            m_ports->setEnabled(false);
            break;
        case DiSEqCDevSwitch::kTypeDiSEqCCommitted:
        case DiSEqCDevSwitch::kTypeDiSEqCUncommitted:
            m_address->setEnabled(true);
            m_ports->setEnabled(true);
            break;
    }
}

//////////////////////////////////////// RotorTypeSetting

class RotorTypeSetting : public ComboBoxSetting, public Storage
{
  public:
    RotorTypeSetting(DiSEqCDevRotor &rotor) :
        ComboBoxSetting(this), m_rotor(rotor)
    {
        setLabel(DeviceTree::tr("Rotor Type"));
        setHelpText(DeviceTree::tr("Select the type of rotor from the list."));
        addSelection(DeviceTree::tr("DiSEqC 1.2"),
                     QString::number((uint) DiSEqCDevRotor::kTypeDiSEqC_1_2));
        addSelection(DeviceTree::tr("DiSEqC 1.3 (GotoX/USALS)"),
                     QString::number((uint) DiSEqCDevRotor::kTypeDiSEqC_1_3));
    }

    virtual void Load(void)
    {
        setValue(getValueIndex(QString::number((uint)m_rotor.GetType())));
    }

    virtual void Save(void)
    {
        m_rotor.SetType((DiSEqCDevRotor::dvbdev_rotor_t)getValue().toUInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevRotor &m_rotor;
};

//////////////////////////////////////// RotorLoSpeedSetting

class RotorLoSpeedSetting : public LineEditSetting, public Storage
{
  public:
    RotorLoSpeedSetting(DiSEqCDevRotor &rotor) :
        LineEditSetting(this), m_rotor(rotor)
    {
        setLabel(DeviceTree::tr("Rotor Low Speed (deg/sec)"));
        QString help = DeviceTree::tr(
            "To allow the approximate monitoring of rotor movement, enter "
            "the rated angular speed of the rotor when powered at 13V.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_rotor.GetLoSpeed()));
    }

    virtual void Save(void)
    {
        m_rotor.SetLoSpeed(getValue().toDouble());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevRotor &m_rotor;
};

//////////////////////////////////////// RotorHiSpeedSetting

class RotorHiSpeedSetting : public LineEditSetting, public Storage
{
  public:
    RotorHiSpeedSetting(DiSEqCDevRotor &rotor) :
        LineEditSetting(this), m_rotor(rotor)
    {
        setLabel(DeviceTree::tr("Rotor High Speed (deg/sec)"));
        QString help = DeviceTree::tr(
            "To allow the approximate monitoring of rotor movement, enter "
            "the rated angular speed of the rotor when powered at 18V.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_rotor.GetHiSpeed()));
    }

    virtual void Save(void)
    {
        m_rotor.SetHiSpeed(getValue().toDouble());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevRotor &m_rotor;
};

//////////////////////////////////////// RotorPosMap

static QString AngleToString(double angle)
{
    QString str = QString::null;
    if (angle >= 0.0)
        str = QString::number(angle) +
            DeviceTree::tr("E", "Eastern Hemisphere");
    else /* if (angle < 0.0) */
        str = QString::number(-angle) +
            DeviceTree::tr("W", "Western Hemisphere");
    return str;
}

static double AngleToEdit(double angle, QString &hemi)
{
    if (angle > 0.0)
    {
        hemi = "E";
        return angle;
    }

    hemi = "W";
    return -angle;
}

static double AngleToFloat(const QString &angle, bool translated = true)
{
    if (angle.length() < 2)
        return 0.0;

    double pos;
    QChar postfix = angle.at(angle.length() - 1);
    if (postfix.isLetter())
    {
        pos = angle.left(angle.length() - 1).toDouble();
        if ((translated &&
             (postfix.toUpper() ==
              DeviceTree::tr("W", "Western Hemisphere")[0])) ||
            (!translated && (postfix.toUpper() == 'W')))
        {
            pos = -pos;
        }
    }
    else
        pos = angle.toDouble();

    return pos;
}

RotorPosMap::RotorPosMap(DiSEqCDevRotor &rotor) :
    ListBoxSetting(this), m_rotor(rotor)
{
    connect(this, SIGNAL(editButtonPressed(int)),   SLOT(edit(void)));
    connect(this, SIGNAL(deleteButtonPressed(int)), SLOT(del(void)));
    connect(this, SIGNAL(accepted(int)),            SLOT(edit(void)));
}

void RotorPosMap::Load(void)
{
    m_posmap = m_rotor.GetPosMap();
    PopulateList();
}

void RotorPosMap::Save(void)
{
    m_rotor.SetPosMap(m_posmap);
}

void RotorPosMap::edit(void)
{
    uint id = getValue().toUInt();

    QString angle;
    if (MythPopupBox::showGetTextPopup(
            GetMythMainWindow(),
            DeviceTree::tr("Position Index %1").arg(id),
            DeviceTree::tr("Orbital Position"), angle))
    {
        m_posmap[id] = AngleToFloat(angle);
        PopulateList();
    }
}

void RotorPosMap::del(void)
{
    uint id = getValue().toUInt();
    m_posmap.erase(m_posmap.find(id));
    PopulateList();
}

void RotorPosMap::PopulateList(void)
{
    int old_sel = getValueIndex(getValue());
    clearSelections();
    uint num_pos = 64;
    for (uint pos = 1; pos < num_pos; pos++)
    {
        uint_to_dbl_t::const_iterator it = m_posmap.find(pos);
        QString posval = DeviceTree::tr("None");
        if (it != m_posmap.end())
            posval = AngleToString(*it);

        addSelection(DeviceTree::tr("Position #%1 (%2)").arg(pos).arg(posval),
                     QString::number(pos));
    }
    setCurrentItem(old_sel);
}

//////////////////////////////////////// RotorPosConfig

class RotorPosConfig : public ConfigurationDialog
{
  public:
    RotorPosConfig(DiSEqCDevRotor &rotor)
    {
        setLabel(DeviceTree::tr("Rotor Position Map"));
        addChild(new RotorPosMap(rotor));
    }

    virtual DialogCode exec(void)
    {
        while (ConfigurationDialog::exec() == kDialogCodeAccepted);
        return kDialogCodeRejected;
    }
    virtual DialogCode exec(bool /*saveOnExec*/, bool /*doLoad*/)
        { return exec(); }
};

//////////////////////////////////////// RotorConfig

RotorConfig::RotorConfig(DiSEqCDevRotor &rotor) : m_rotor(rotor)
{
    ConfigurationGroup *group =
        new VerticalConfigurationGroup(false, false);
    group->setLabel(DeviceTree::tr("Rotor Configuration"));

    group->addChild(new DeviceDescrSetting(rotor));
    group->addChild(new DeviceRepeatSetting(rotor));

    ConfigurationGroup *tgroup =
        new HorizontalConfigurationGroup(false, false, true, true);

    RotorTypeSetting *rtype = new RotorTypeSetting(rotor);
    connect(rtype, SIGNAL(valueChanged(const QString&)),
            this,  SLOT(  SetType(     const QString&)));
    tgroup->addChild(rtype);

    m_pos = new TransButtonSetting();
    m_pos->setLabel(DeviceTree::tr("Positions"));
    m_pos->setHelpText(DeviceTree::tr("Rotor position setup."));
    m_pos->setEnabled(rotor.GetType() == DiSEqCDevRotor::kTypeDiSEqC_1_2);
    connect(m_pos, SIGNAL(pressed(void)),
            this,  SLOT(  RunRotorPositionsDialog(void)));
    tgroup->addChild(m_pos);

    group->addChild(tgroup);
    group->addChild(new RotorLoSpeedSetting(rotor));
    group->addChild(new RotorHiSpeedSetting(rotor));
    group->addChild(DiSEqCLatitude());
    group->addChild(DiSEqCLongitude());

    addChild(group);
}

void RotorConfig::SetType(const QString &type)
{
    DiSEqCDevRotor::dvbdev_rotor_t rtype;
    rtype = (DiSEqCDevRotor::dvbdev_rotor_t) type.toUInt();
    m_pos->setEnabled(rtype == DiSEqCDevRotor::kTypeDiSEqC_1_2);
}

void RotorConfig::RunRotorPositionsDialog(void)
{
    RotorPosConfig config(m_rotor);
    config.exec();
    config.Save();
}

//////////////////////////////////////// SCRUserBandSetting

class SCRUserBandSetting : public SpinBoxSetting, public Storage
{
  public:
    SCRUserBandSetting(DiSEqCDevSCR &scr) :
        SpinBoxSetting(this, 0, 8, 1), m_scr(scr)
    {
        setLabel(DeviceTree::tr("Userband"));
        setHelpText(DeviceTree::tr("Unicable userband ID (0-7) or sometimes (1-8)"));
    }

    virtual void Load(void)
    {
        setValue(m_scr.GetUserBand());
    }

    virtual void Save(void)
    {
        m_scr.SetUserBand(intValue());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevSCR &m_scr;
};

//////////////////////////////////////// SCRFrequencySetting

class SCRFrequencySetting : public LineEditSetting, public Storage
{
  public:
    SCRFrequencySetting(DiSEqCDevSCR &scr) : LineEditSetting(this), m_scr(scr)
    {
        setLabel(DeviceTree::tr("Frequency (MHz)"));
        setHelpText(DeviceTree::tr("Unicable userband frequency (usually 1210, 1420, 1680 and 2040 MHz)"));
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_scr.GetFrequency()));
    }

    virtual void Save(void)
    {
        m_scr.SetFrequency(getValue().toUInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevSCR &m_scr;
};

//////////////////////////////////////// SCRPINSetting

class SCRPINSetting : public LineEditSetting, public Storage
{
  public:
    SCRPINSetting(DiSEqCDevSCR &scr) : LineEditSetting(this), m_scr(scr)
    {
        setLabel(DeviceTree::tr("PIN code"));
        setHelpText(DeviceTree::tr("Unicable PIN code (-1 disabled, 0 - 255)"));
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_scr.GetPIN()));
    }

    virtual void Save(void)
    {
        m_scr.SetPIN(getValue().toInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevSCR &m_scr;
};

//////////////////////////////////////// SCRConfig

SCRConfig::SCRConfig(DiSEqCDevSCR &scr) : m_scr(scr)
{
    ConfigurationGroup *group =
        new VerticalConfigurationGroup(false, false);
    group->setLabel(DeviceTree::tr("Unicable Configuration"));

    group->addChild(new SCRUserBandSetting(scr));
    group->addChild(new SCRFrequencySetting(scr));
    group->addChild(new SCRPINSetting(scr));
    group->addChild(new DeviceRepeatSetting(scr));

    addChild(group);
}

//////////////////////////////////////// LnbPresetSetting

class lnb_preset
{
  public:
    lnb_preset(const QString &_name, DiSEqCDevLNB::dvbdev_lnb_t _type,
               uint _lof_sw = 0, uint _lof_lo = 0,
               uint _lof_hi = 0, bool _pol_inv = false) :
        name(_name),     type(_type),
        lof_sw(_lof_sw), lof_lo(_lof_lo),
        lof_hi(_lof_hi), pol_inv(_pol_inv) {}

  public:
    QString                    name;
    DiSEqCDevLNB::dvbdev_lnb_t type;
    uint                       lof_sw;
    uint                       lof_lo;
    uint                       lof_hi;
    bool                       pol_inv;
};

static lnb_preset lnb_presets[] =
{

    /* description, type, LOF switch, LOF low, LOF high, inverted polarity */
    lnb_preset(QT_TRANSLATE_NOOP("DeviceTree", "Universal (Europe)"),
               DiSEqCDevLNB::kTypeVoltageAndToneControl,
               11700000,  9750000, 10600000),
    lnb_preset(QT_TRANSLATE_NOOP("DeviceTree", "Single (Europe)"),
               DiSEqCDevLNB::kTypeVoltageControl,       0,  9750000),
    lnb_preset(QT_TRANSLATE_NOOP("DeviceTree", "Circular (N. America)"),
               DiSEqCDevLNB::kTypeVoltageControl,       0, 11250000),
    lnb_preset(QT_TRANSLATE_NOOP("DeviceTree", "Linear (N. America)"),
               DiSEqCDevLNB::kTypeVoltageControl,       0, 10750000),
    lnb_preset(QT_TRANSLATE_NOOP("DeviceTree", "C Band"),
               DiSEqCDevLNB::kTypeVoltageControl,       0,  5150000),
    lnb_preset(QT_TRANSLATE_NOOP("DeviceTree", "DishPro Bandstacked"),
               DiSEqCDevLNB::kTypeBandstacked,          0, 11250000, 14350000),
    lnb_preset(QString::null, DiSEqCDevLNB::kTypeVoltageControl),
};

static uint FindPreset(const DiSEqCDevLNB &lnb)
{
    uint i;
    for (i = 0; !lnb_presets[i].name.isEmpty(); i++)
    {
        if (lnb_presets[i].type   == lnb.GetType()      &&
            lnb_presets[i].lof_sw == lnb.GetLOFSwitch() &&
            lnb_presets[i].lof_lo == lnb.GetLOFLow()    &&
            lnb_presets[i].lof_hi == lnb.GetLOFHigh()   &&
            lnb_presets[i].pol_inv== lnb.IsPolarityInverted())
        {
            break;
        }
    }
    return i;
}

class LNBPresetSetting : public ComboBoxSetting, public Storage
{
  public:
    LNBPresetSetting(DiSEqCDevLNB &lnb) : ComboBoxSetting(this), m_lnb(lnb)
    {
        setLabel(DeviceTree::tr("LNB Preset"));
        QString help = DeviceTree::tr(
            "Select the LNB preset from the list, or choose "
            "'Custom' and set the advanced settings below.");
        setHelpText(help);

        uint i = 0;
        for (; !lnb_presets[i].name.isEmpty(); i++)
                        addSelection(DeviceTree::tr( lnb_presets[i].name.toUtf8() ),
                         QString::number(i));
        addSelection(DeviceTree::tr("Custom"), QString::number(i));
    }

    virtual void Load(void)
    {
        setValue(FindPreset(m_lnb));
    }

    virtual void Save(void)
    {
    }

    virtual void Save(QString /*destination*/)
    {
    }

  private:
    DiSEqCDevLNB &m_lnb;
};

//////////////////////////////////////// LNBTypeSetting

class LNBTypeSetting : public ComboBoxSetting, public Storage
{
  public:
    LNBTypeSetting(DiSEqCDevLNB &lnb) : ComboBoxSetting(this), m_lnb(lnb)
    {
        setLabel(DeviceTree::tr("LNB Type"));
        setHelpText(DeviceTree::tr("Select the type of LNB from the list."));
        addSelection(DeviceTree::tr("Legacy (Fixed)"),
                     QString::number((uint) DiSEqCDevLNB::kTypeFixed));
        addSelection(DeviceTree::tr("Standard (Voltage)"),
                     QString::number((uint) DiSEqCDevLNB::
                                     kTypeVoltageControl));
        addSelection(DeviceTree::tr("Universal (Voltage & Tone)"),
                     QString::number((uint) DiSEqCDevLNB::
                                     kTypeVoltageAndToneControl));
        addSelection(DeviceTree::tr("Bandstacked"),
                     QString::number((uint) DiSEqCDevLNB::kTypeBandstacked));
    }

    virtual void Load(void)
    {
        setValue(getValueIndex(QString::number((uint) m_lnb.GetType())));
    }

    virtual void Save(void)
    {
        m_lnb.SetType((DiSEqCDevLNB::dvbdev_lnb_t) getValue().toUInt());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevLNB &m_lnb;
};

//////////////////////////////////////// LNBLOFSwitchSetting

class LNBLOFSwitchSetting : public LineEditSetting, public Storage
{
  public:
    LNBLOFSwitchSetting(DiSEqCDevLNB &lnb) : LineEditSetting(this), m_lnb(lnb)
    {
        setLabel(DeviceTree::tr("LNB LOF Switch (MHz)"));
        QString help = DeviceTree::tr(
            "This defines at what frequency the LNB will do a "
            "switch from high to low setting, and vice versa.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_lnb.GetLOFSwitch() / 1000));
    }

    virtual void Save(void)
    {
        m_lnb.SetLOFSwitch(getValue().toUInt() * 1000);
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevLNB &m_lnb;
};

//////////////////////////////////////// LNBLOFLowSetting

class LNBLOFLowSetting : public LineEditSetting, public Storage
{
  public:
    LNBLOFLowSetting(DiSEqCDevLNB &lnb) : LineEditSetting(this), m_lnb(lnb)
    {
        setLabel(DeviceTree::tr("LNB LOF Low (MHz)"));
        QString help = DeviceTree::tr(
            "This defines the offset the frequency coming "
            "from the LNB will be in low setting. For bandstacked "
            "LNBs this is the vertical/right polarization band.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_lnb.GetLOFLow() / 1000));
    }

    virtual void Save(void)
    {
        m_lnb.SetLOFLow(getValue().toUInt() * 1000);
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevLNB &m_lnb;
};

//////////////////////////////////////// LNBLOFHighSetting

class LNBLOFHighSetting : public LineEditSetting, public Storage
{
  public:
    LNBLOFHighSetting(DiSEqCDevLNB &lnb) : LineEditSetting(this), m_lnb(lnb)
    {
        setLabel(DeviceTree::tr("LNB LOF High (MHz)"));
        QString help = DeviceTree::tr(
            "This defines the offset the frequency coming from "
            "the LNB will be in high setting. For bandstacked "
            "LNBs this is the horizontal/left polarization band.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(QString::number(m_lnb.GetLOFHigh() / 1000));
    }

    virtual void Save(void)
    {
        m_lnb.SetLOFHigh(getValue().toUInt() * 1000);
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevLNB &m_lnb;
};

class LNBPolarityInvertedSetting : public CheckBoxSetting, public Storage
{
  public:
    LNBPolarityInvertedSetting(DiSEqCDevLNB &lnb) :
        CheckBoxSetting(this), m_lnb(lnb)
    {
        setLabel(DeviceTree::tr("LNB Reversed"));
        QString help = DeviceTree::tr(
            "This defines whether the signal reaching the LNB "
            "is reversed from normal polarization. This happens "
            "to circular signals bouncing twice on a toroidal "
            "dish.");
        setHelpText(help);
    }

    virtual void Load(void)
    {
        setValue(m_lnb.IsPolarityInverted());
    }

    virtual void Save(void)
    {
        m_lnb.SetPolarityInverted(boolValue());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevLNB &m_lnb;
};

//////////////////////////////////////// LNBConfig

LNBConfig::LNBConfig(DiSEqCDevLNB &lnb)
{
    ConfigurationGroup *group =
        new VerticalConfigurationGroup(false, false);
    group->setLabel(DeviceTree::tr("LNB Configuration"));

    group->addChild(new DeviceDescrSetting(lnb));
    LNBPresetSetting *preset = new LNBPresetSetting(lnb);
    group->addChild(preset);
    m_type = new LNBTypeSetting(lnb);
    group->addChild(m_type);
    m_lof_switch = new LNBLOFSwitchSetting(lnb);
    group->addChild(m_lof_switch);
    m_lof_lo = new LNBLOFLowSetting(lnb);
    group->addChild(m_lof_lo);
    m_lof_hi = new LNBLOFHighSetting(lnb);
    group->addChild(m_lof_hi);
    m_pol_inv = new LNBPolarityInvertedSetting(lnb);
    group->addChild(m_pol_inv);
    connect(m_type, SIGNAL(valueChanged(const QString&)),
            this,   SLOT(  UpdateType(  void)));
    connect(preset, SIGNAL(valueChanged(const QString&)),
            this,   SLOT(  SetPreset(   const QString&)));
    addChild(group);
}

void LNBConfig::SetPreset(const QString &value)
{
    uint index = value.toUInt();
    if (index >= (sizeof(lnb_presets) / sizeof(lnb_preset)))
        return;

    lnb_preset &preset = lnb_presets[index];
    if (preset.name.isEmpty())
    {
        m_type->setEnabled(true);
        UpdateType();
    }
    else
    {
        m_type->setValue(m_type->getValueIndex(
                             QString::number((uint)preset.type)));
        m_lof_switch->setValue(QString::number(preset.lof_sw / 1000));
        m_lof_lo->setValue(QString::number(preset.lof_lo / 1000));
        m_lof_hi->setValue(QString::number(preset.lof_hi / 1000));
        m_pol_inv->setValue(preset.pol_inv);
        m_type->setEnabled(false);
        m_lof_switch->setEnabled(false);
        m_lof_hi->setEnabled(false);
        m_lof_lo->setEnabled(false);
        m_pol_inv->setEnabled(false);
    }
}

void LNBConfig::UpdateType(void)
{
    if (!m_type->isEnabled())
        return;

    switch ((DiSEqCDevLNB::dvbdev_lnb_t) m_type->getValue().toUInt())
    {
        case DiSEqCDevLNB::kTypeFixed:
        case DiSEqCDevLNB::kTypeVoltageControl:
            m_lof_switch->setEnabled(false);
            m_lof_hi->setEnabled(false);
            m_lof_lo->setEnabled(true);
            m_pol_inv->setEnabled(true);
            break;
        case DiSEqCDevLNB::kTypeVoltageAndToneControl:
            m_lof_switch->setEnabled(true);
            m_lof_hi->setEnabled(true);
            m_lof_lo->setEnabled(true);
            m_pol_inv->setEnabled(true);
            break;
        case DiSEqCDevLNB::kTypeBandstacked:
            m_lof_switch->setEnabled(false);
            m_lof_hi->setEnabled(true);
            m_lof_lo->setEnabled(true);
            m_pol_inv->setEnabled(true);
            break;
    }
}

//////////////////////////////////////// DeviceTree

DeviceTree::DeviceTree(DiSEqCDevTree &tree) :
    ListBoxSetting(this), m_tree(tree)
{
    connect(this, SIGNAL(editButtonPressed(int)),   SLOT(edit(void)));
    connect(this, SIGNAL(deleteButtonPressed(int)), SLOT(del(void)));
    connect(this, SIGNAL(accepted(int)),            SLOT(edit(void)));
}

void DeviceTree::Load(void)
{
    PopulateTree();
}

void DeviceTree::Save(void)
{
}

bool DeviceTree::EditNodeDialog(uint nodeid)
{
    DiSEqCDevDevice *dev = m_tree.FindDevice(nodeid);
    if (!dev)
    {
        LOG(VB_GENERAL, LOG_ERR, QString("DeviceTree::EditNodeDialog(%1) "
                                         "-- device not found").arg(nodeid));
        return false;
    }

    bool changed = false;
    switch (dev->GetDeviceType())
    {
        case DiSEqCDevDevice::kTypeSwitch:
        {
            DiSEqCDevSwitch *sw = dynamic_cast<DiSEqCDevSwitch*>(dev);
            if (sw)
            {
                SwitchConfig config(*sw);
                changed = (config.exec() == MythDialog::Accepted);
            }
        }
        break;

        case DiSEqCDevDevice::kTypeRotor:
        {
            DiSEqCDevRotor *rotor = dynamic_cast<DiSEqCDevRotor*>(dev);
            if (rotor)
            {
                RotorConfig config(*rotor);
                changed = (config.exec() == MythDialog::Accepted);
            }
        }
        break;

        case DiSEqCDevDevice::kTypeSCR:
        {
            DiSEqCDevSCR *scr = dynamic_cast<DiSEqCDevSCR*>(dev);
            if (scr)
            {
                SCRConfig config(*scr);
                changed = (config.exec() == MythDialog::Accepted);
            }
        }
        break;

        case DiSEqCDevDevice::kTypeLNB:
        {
            DiSEqCDevLNB *lnb = dynamic_cast<DiSEqCDevLNB*>(dev);
            if (lnb)
            {
                LNBConfig config(*lnb);
                changed = (config.exec() == MythDialog::Accepted);
            }
        }
        break;

        default:
            break;
    }

    if (changed)
        PopulateTree();

    return changed;
}

bool DeviceTree::RunTypeDialog(DiSEqCDevDevice::dvbdev_t &type)
{
    MythPopupBox *popup = new MythPopupBox(GetMythMainWindow(), "");
    popup->addLabel(tr("Select Type of Device"));

    MythListBox *list = new MythListBox(popup);
    list->insertItem(tr("Switch"));
    list->insertItem(tr("Rotor"));
    list->insertItem(tr("Unicable"));
    list->insertItem(tr("LNB"));
    list->setCurrentRow(0, QItemSelectionModel::Select);

    popup->addWidget(list);
    connect(list,  SIGNAL(accepted(int)),
            popup, SLOT(  AcceptItem(int)));
    list->setFocus();

    DialogCode res = popup->ExecPopup();
    type = (DiSEqCDevDevice::dvbdev_t)(list->currentRow());

    popup->hide();
    popup->deleteLater();

    return kDialogCodeRejected != res;
}

void DeviceTree::CreateRootNodeDialog(void)
{
    DiSEqCDevDevice::dvbdev_t type;
    if (!RunTypeDialog(type))
        return;

    DiSEqCDevDevice *dev = DiSEqCDevDevice::CreateByType(m_tree, type);
    if (dev)
    {
        m_tree.SetRoot(dev);

        if (!EditNodeDialog(dev->GetDeviceID()))
            m_tree.SetRoot(NULL);

        PopulateTree();
    }
}

void DeviceTree::CreateNewNodeDialog(uint parentid, uint child_num)
{
    DiSEqCDevDevice *parent = m_tree.FindDevice(parentid);
    if (!parent)
        return;

    DiSEqCDevDevice::dvbdev_t type;
    if (RunTypeDialog(type))
    {
        DiSEqCDevDevice *dev = DiSEqCDevDevice::CreateByType(m_tree, type);
        if (!dev)
            return;

        if (parent->SetChild(child_num, dev))
        {
            if (!EditNodeDialog(dev->GetDeviceID()))
                parent->SetChild(child_num, NULL);
            PopulateTree();
        }
        else
        {
            delete dev;
        }
    }
}

void DeviceTree::edit(void)
{
    QString id = getValue();
    if (id.indexOf(':') == -1)
    {
        EditNodeDialog(id.toUInt());
    }
    else
    {
        QStringList vals = id.split(':');
        if (vals[0].isEmpty())
            CreateRootNodeDialog();
        else
            CreateNewNodeDialog(vals[0].toUInt(), vals[1].toUInt());
    }
    setFocus();
}

void DeviceTree::del(void)
{
    QString id = getValue();

    if (id.indexOf(':') == -1)
    {
        uint nodeid = id.toUInt();
        DiSEqCDevDevice *dev = m_tree.FindDevice(nodeid);
        if (dev)
        {
            DiSEqCDevDevice *parent = dev->GetParent();
            if (parent)
                parent->SetChild(dev->GetOrdinal(), NULL);
            else
                m_tree.SetRoot(NULL);

            PopulateTree();
        }
    }

    setFocus();
}

void DeviceTree::PopulateTree(void)
{
    int old_sel = getValueIndex(getValue());
    clearSelections();
    PopulateTree(m_tree.Root());
    setCurrentItem(old_sel);
}

void DeviceTree::PopulateTree(DiSEqCDevDevice *node,
                              DiSEqCDevDevice *parent,
                              uint childnum,
                              uint depth)
{
    QString indent;
    indent.fill(' ', 8 * depth);

    if (node)
    {
        QString id = QString::number(node->GetDeviceID());
        addSelection(indent + node->GetDescription(), id);
        uint num_ch = node->GetChildCount();
        for (uint ch = 0; ch < num_ch; ch++)
            PopulateTree(node->GetChild(ch), node, ch, depth+1);
    }
    else
    {
        QString id;
        if (parent)
            id = QString::number(parent->GetDeviceID());
        id += ":" + QString::number(childnum);

        addSelection(indent + "(Unconnected)", id);
    }
}

//////////////////////////////////////// DTVDeviceTreeWizard

DTVDeviceTreeWizard::DTVDeviceTreeWizard(DiSEqCDevTree &tree)
{
    setLabel(DeviceTree::tr("DiSEqC Device Tree"));
    addChild(new DeviceTree(tree));
}

DialogCode DTVDeviceTreeWizard::exec(void)
{
    while (ConfigurationDialog::exec() == kDialogCodeAccepted);
    return kDialogCodeRejected;
}

//////////////////////////////////////// SwitchSetting

class SwitchSetting : public ComboBoxSetting, public Storage
{
  public:
    SwitchSetting(DiSEqCDevDevice &node, DiSEqCDevSettings &settings)
        : ComboBoxSetting(this), m_node(node), m_settings(settings)
    {
        setLabel(node.GetDescription());
        setHelpText(DeviceTree::tr("Choose a port to use for this switch."));

        uint num_children = node.GetChildCount();
        for (uint ch = 0; ch < num_children; ch++)
        {
            QString val = QString("%1").arg(ch);
            QString descr = DeviceTree::tr("Port %1").arg(ch+1);
            DiSEqCDevDevice *child = node.GetChild(ch);
            if (child)
                descr += QString(" (%2)").arg(child->GetDescription());
            addSelection(descr, val);
        }
    }

    virtual void Load(void)
    {
        double value = m_settings.GetValue(m_node.GetDeviceID());
        setValue((uint)value);
    }

    virtual void Save(void)
    {
        m_settings.SetValue(m_node.GetDeviceID(), getValue().toDouble());
    }

    virtual void Save(QString /*destination*/) {}

  private:
    DiSEqCDevDevice   &m_node;
    DiSEqCDevSettings &m_settings;
};

//////////////////////////////////////// RotorSetting

class RotorSetting : public ComboBoxSetting, public Storage
{
  public:
    RotorSetting(DiSEqCDevDevice &node, DiSEqCDevSettings &settings)
        : ComboBoxSetting(this), m_node(node), m_settings(settings)
    {
        setLabel(node.GetDescription());
        setHelpText(DeviceTree::tr("Choose a satellite position."));

        DiSEqCDevRotor *rotor = dynamic_cast<DiSEqCDevRotor*>(&m_node);
        if (rotor)
            m_posmap = rotor->GetPosMap();
    }

    virtual void Load(void)
    {
        clearSelections();

        uint_to_dbl_t::const_iterator it;
        for (it = m_posmap.begin(); it != m_posmap.end(); ++it)
            addSelection(AngleToString(*it), QString::number(*it));

        double angle = m_settings.GetValue(m_node.GetDeviceID());
        setValue(getValueIndex(QString::number(angle)));
    }

    virtual void Save(void)
    {
        m_settings.SetValue(m_node.GetDeviceID(), getValue().toDouble());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevDevice   &m_node;
    DiSEqCDevSettings &m_settings;
    uint_to_dbl_t      m_posmap;
};

//////////////////////////////////////// USALSRotorSetting

class USALSRotorSetting : public HorizontalConfigurationGroup
{
  public:
    USALSRotorSetting(DiSEqCDevDevice &node, DiSEqCDevSettings &settings) :
        HorizontalConfigurationGroup(false, false, true, true),
        numeric(new TransLineEditSetting()),
        hemisphere(new TransComboBoxSetting(false)),
        m_node(node), m_settings(settings)
    {
        QString help =
            DeviceTree::tr(
                "Locates the satellite you wish to point to "
                "with the longitude along the Clarke Belt of "
                "the satellite [-180..180] and its hemisphere.");

        numeric->setLabel(DeviceTree::tr("Longitude (degrees)"));
        numeric->setHelpText(help);
        hemisphere->setLabel(DeviceTree::tr("Hemisphere"));
        hemisphere->addSelection(DeviceTree::tr("Eastern"), "E", false);
        hemisphere->addSelection(DeviceTree::tr("Western"), "W", true);
        hemisphere->setHelpText(help);

        addChild(numeric);
        addChild(hemisphere);
    }

    virtual void Load(void)
    {
        double  val  = m_settings.GetValue(m_node.GetDeviceID());
        QString hemi = QString::null;
        double  eval = AngleToEdit(val, hemi);
        numeric->setValue(QString::number(eval));
        hemisphere->setValue(hemisphere->getValueIndex(hemi));
    }

    virtual void Save(void)
    {
        QString val = QString::number(numeric->getValue().toDouble());
        val += hemisphere->getValue();
        m_settings.SetValue(m_node.GetDeviceID(), AngleToFloat(val, false));
    }

    virtual void Save(QString /*destination*/) { }

  private:
    TransLineEditSetting *numeric;
    TransComboBoxSetting *hemisphere;
    DiSEqCDevDevice      &m_node;
    DiSEqCDevSettings    &m_settings;
};

//////////////////////////////////////// SCRPositionSetting

class SCRPositionSetting : public ComboBoxSetting, public Storage
{
  public:
    SCRPositionSetting(DiSEqCDevDevice &node, DiSEqCDevSettings &settings)
        : ComboBoxSetting(this), m_node(node), m_settings(settings)
    {
        setLabel("Position");
        setHelpText(DeviceTree::tr("Unicable satellite position (A/B)"));
        addSelection(DiSEqCDevSCR::SCRPositionToString(DiSEqCDevSCR::kTypeScrPosA),
                     QString::number((uint)DiSEqCDevSCR::kTypeScrPosA), true);
        addSelection(DiSEqCDevSCR::SCRPositionToString(DiSEqCDevSCR::kTypeScrPosB),
                     QString::number((uint)DiSEqCDevSCR::kTypeScrPosB), false);
    }

    virtual void Load(void)
    {
        double value = m_settings.GetValue(m_node.GetDeviceID());
        setValue(getValueIndex(QString::number((uint)value)));
    }

    virtual void Save(void)
    {
        m_settings.SetValue(m_node.GetDeviceID(), getValue().toDouble());
    }

    virtual void Save(QString /*destination*/) { }

  private:
    DiSEqCDevDevice      &m_node;
    DiSEqCDevSettings    &m_settings;
};

//////////////////////////////////////// DTVDeviceConfigGroup

DTVDeviceConfigGroup::DTVDeviceConfigGroup(
    DiSEqCDevSettings &settings, uint cardid, bool switches_enabled) :
    VerticalConfigurationGroup(false, false, true, true),
    m_settings(settings), m_switches_enabled(switches_enabled)
{
    setLabel(DeviceTree::tr("DTV Device Configuration"));

    // load
    m_tree.Load(cardid);

    // initial UI setup
    AddNodes(this, QString::null, m_tree.Root());
}

DTVDeviceConfigGroup::~DTVDeviceConfigGroup(void)
{
}

void DTVDeviceConfigGroup::AddNodes(
    ConfigurationGroup *group, const QString &trigger, DiSEqCDevDevice *node)
{
    if (!node)
        return;

    Setting *setting = NULL;
    switch (node->GetDeviceType())
    {
        case DiSEqCDevDevice::kTypeSwitch:
            setting = new SwitchSetting(*node, m_settings);
            setting->setEnabled(m_switches_enabled);
            break;
        case DiSEqCDevDevice::kTypeRotor:
        {
            DiSEqCDevRotor *rotor = dynamic_cast<DiSEqCDevRotor*>(node);
            if (rotor && (rotor->GetType() == DiSEqCDevRotor::kTypeDiSEqC_1_2))
                setting = new RotorSetting(*node, m_settings);
            else
                setting = new USALSRotorSetting(*node, m_settings);
            break;
        }
        case DiSEqCDevDevice::kTypeSCR:
        {
            setting = new SCRPositionSetting(*node, m_settings);
            break;
        }
        default:
            break;
    }

    if (!setting)
    {
        AddChild(group, trigger, new TransLabelSetting());
        return;
    }

    m_devs[node->GetDeviceID()] = setting;

    uint num_ch = node->GetChildCount();
    if (DiSEqCDevDevice::kTypeSwitch == node->GetDeviceType())
    {
        bool useframe = (node != m_tree.Root());
        bool zerospace = !useframe;
        TriggeredConfigurationGroup *cgrp = new TriggeredConfigurationGroup(
            false, useframe, true, true, false, false, true, zerospace);

        cgrp->addChild(setting);
        cgrp->setTrigger(setting);

        for (uint i = 0; i < num_ch; i++)
            AddNodes(cgrp, QString::number(i), node->GetChild(i));

        AddChild(group, trigger, cgrp);
        return;
    }

    if (!num_ch)
    {
        AddChild(group, trigger, setting);
        return;
    }

    VerticalConfigurationGroup *cgrp =
        new VerticalConfigurationGroup(false, false, true, true);

    AddChild(cgrp, QString::null, setting);
    for (uint i = 0; i < num_ch; i++)
        AddNodes(cgrp, QString::null, node->GetChild(i));

    AddChild(group, trigger, cgrp);
}

void DTVDeviceConfigGroup::AddChild(
    ConfigurationGroup *group, const QString &trigger, Setting *setting)
{
    TriggeredConfigurationGroup *grp =
        dynamic_cast<TriggeredConfigurationGroup*>(group);

    if (grp && !trigger.isEmpty())
        grp->addTarget(trigger, setting);
    else
        group->addChild(setting);
}
