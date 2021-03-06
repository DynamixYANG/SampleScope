/*

SampleScope: A humble USB oscilloscope
Copyright (C) 2012 mru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef DEVICE_H
#define DEVICE_H

#include <vector>
#include <QObject>
#include <QPointF>
#include <QVector>
#include "../../shared/common.h"
#include "abstracthardware.h"

#define MAX_STR 65

class Device : public QObject
{
        typedef float sample_t;
        Q_OBJECT

public:
        typedef unsigned short triggerLevel_t;

        enum {
                RX_SIZE = 10
        };
public:
        explicit Device(QObject *parent = 0);
        ~Device();

        bool isConnected() { return device->isOpen(); }
        void connect();
        void disConnect();

        void selectHardwareImplementation(AbstractHardware::Impl_t);


        void setVdiv_Ch1(DeviceConstants::VdivValues_t);
        void setVdiv_Ch2(DeviceConstants::VdivValues_t);
        void setTdiv(DeviceConstants::TdivValues_t);
        void setDelay(unsigned short);
        void ping();
        void setTriggerLevel(triggerLevel_t level);
        sample_t getADCSingle();
        void getADCBlock(DeviceConstants::Channel_t ch, QVector<QPointF>&);

        void getADCInterleaved(QVector<QPointF>& ch1, QVector<QPointF>& ch2);
        void setACDC_Ch1(DeviceConstants::ACDC_t ch1);
        void setACDC_Ch2(DeviceConstants::ACDC_t ch2);
        void selectTriggerSource(DeviceConstants::TriggerSource_t trigger_source);
        DeviceConstants::TriggerSource_t getTriggerSource() { return config.triggerChannel; }
        void selectTriggerMode(DeviceConstants::TriggerMode_t);


        static QString getTdivUnit(DeviceConstants::TdivValues_t);
        static QString getTdivLabel(DeviceConstants::TdivValues_t);
        static QString getVdivLabel(DeviceConstants::VdivValues_t);
        static QString getChannelName(DeviceConstants::Channel_t);

        static double getVdivVoltate(DeviceConstants::VdivValues_t vdiv, double val);
        static double getTdivTime(DeviceConstants::TdivValues_t tdiv, double val);

        void setTestSignal(DeviceConstants::TestSignal_t);
signals:
        void connected(QString manufacturer, QString product);
        void disconnected();
        void fatal(QString message);

public slots:
        void refresh();

private:
        AbstractHardware* device;
        unsigned char buf[MAX_STR];

        sample_t normalizeSample(unsigned char val) {
                return 2.05 * (val - 128) / 128.0;
        }
        void comm(const DeviceConstants::opcodes_t command);
        DeviceConstants::osci_config_t config;
        DeviceConstants::device_info_t deviceInfo;
        void transmitConfig();
        void receiveInfo();
        DeviceConstants::TestSignal_t dummy;

        unsigned short delay_cal[DeviceConstants::Tdiv_LAST+1];
        unsigned short gain_ch1_cal[DeviceConstants::Vdiv_LAST+1];
        unsigned short gain_ch2_cal[DeviceConstants::Vdiv_LAST+1];
};

#endif // DEVICE_H
