#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>

int main(int ac, char *av[])
{
	int uid = getuid();

	struct passwd *pw = getpwuid(uid);
	if (pw == NULL) {
		perror("getpwuid error: ");
	}

	int ngroups = 0;

	printf("uid=%d(%s) ", uid, pw->pw_name);
	printf("gid=%d(%s) ", pw->pw_gid, pw->pw_name);

	getgrouplist(pw->pw_name, pw->pw_gid, NULL, &ngroups);
	__gid_t groups[ngroups];

	getgrouplist(pw->pw_name, pw->pw_gid, groups, &ngroups);

	printf("groups=");

	for (int i = 0; i < ngroups; i++) {
		struct group *gr = getgrgid(groups[i]);
		if (gr == NULL) {
			perror("getgrgid error: ");
		}
		printf("%d(%s)", groups[i], gr->gr_name);
		if (i != (ngroups - 1)) {
			putchar(',');
		}
	}
	putchar('\n');
}
