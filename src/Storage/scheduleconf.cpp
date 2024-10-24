#include "scheduleconf.h"


ScheduleConf::ScheduleConf()
{
    /* */
}

ScheduleConf::~ScheduleConf()
{
    /* */
}

void ScheduleConf::init()
{
    store = new Preferences();
    store->begin(_nameSpace.c_str(), false);
    // Check if it is the first time it runs and there is not data saved.
    if (!store->isKey(this->_zoneNames[0].c_str()))
    {
        // if not data saved, save default values
        for (int i = 0; i < MAX_ZONES; i++)
        {
            store->putBytes(this->_zoneNames[i].c_str(), &this->_zones[i], sizeof(schedule_t));
        }
    }
    else
    {
        // Data exist, store it in RAM
        for (int i = 0; i < MAX_ZONES; i++)
        {
            store->getBytes(this->_zoneNames[i].c_str(), &this->_zones[i], sizeof(schedule_t));
        }
    }
    // close the namespace in case some other code wants to save data.
    store->end();
}

schedule_t ScheduleConf::getConf(int zone)
{
        return this->_zones[zone];
}

void ScheduleConf::setConf(int zone, schedule_t *conf)
{
    // if the value is the same as the previos stored, do not do anything
    if(!memcmp(&this->_zones[zone],conf,sizeof(schedule_t)))
    {
        return;
    }
    store->begin(_nameSpace.c_str(), false);
    store->putBytes(this->_zoneNames[zone].c_str(), conf, sizeof(schedule_t));
    this->_zones[zone] = *conf;
    store->end();
}
