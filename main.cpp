#include <string.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "ccronexpr.h"

using namespace std;

int gettimezone() {
	time_t time_utc = 0;
	struct tm p_tm_time;
	int time_zone = 0;
	localtime_r(&time_utc, &p_tm_time);
	time_zone = ( p_tm_time.tm_hour > 12 ) ? ( p_tm_time.tm_hour-= 24 ) : p_tm_time.tm_hour;
	cout << "time_zone: " <<time_zone *3600 << endl;
	return time_zone * 3600;
}

int main() {
	cron_expr expr;
	const char* err = NULL;
	memset(&expr, 0, sizeof(expr));
	cron_parse_expr("0 30 9 27 2 * *", &expr, &err);
	//cron_parse_expr("0 * 9 * * ? 119-122", &expr, &err);
	if (err) {
		printf("err: %s\n", err);
		return -1;
	}

	struct tm * local_cur = NULL;
	char stime[128] = {0};

	//time_t cur_utc = time(NULL);
	time_t cur_utc = 1644219935;

	printf("%lld\n", cur_utc);
	
	local_cur = localtime(&cur_utc);

	time_t cur = cur_utc + gettimezone();

	printf("%lld\n", cur);

	// time_t cur = time(NULL);
	strftime(stime, sizeof(stime), "%Y-%m-%d %H:%M:%S", local_cur);
	printf("cur : %s\n", stime);

	time_t next = cron_next(&expr, cur) - gettimezone();
	time_t prev = cron_prev(&expr, cur); // prev有bug，无法区分时区

	struct tm * local_time = NULL;
	
	local_time = localtime(&next);
	strftime(stime, sizeof(stime), "%Y-%m-%d %H:%M:%S", local_time);
	printf("next: %s\n", stime);

	local_time = localtime(&prev);
	strftime(stime, sizeof(stime), "%Y-%m-%d %H:%M:%S", local_time);
	printf("prev: %s\n", stime);
	return 0;
}
