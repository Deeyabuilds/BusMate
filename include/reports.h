#ifndef REPORTS_H
#define REPORTS_H

/* Reports Menu */
void reportsMenu(void);

/* Bus Reports */
void generateDailyBusReport(void);
void generateBusOccupancyReport(void);

/* Route Reports */
void generateRouteWiseReport(void);

/* Schedule Reports */
void generateFullDayReport(void);
void generateHalfDayReport(void);

/* Driver and Student Reports */
void generateDriverReport(void);
void generateStudentSeatReport(void);

#endif