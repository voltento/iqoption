import sys
import datetime

if len(sys.argv) != 3:
    print("Expected args: [num_users] [out_file]")
    exit(1)

numUsers = int(sys.argv[1])

with open(sys.argv[2], "w") as out:
    for i in range(numUsers):
        out.write(' '.join(("user_registered", str(i), "user" + str(i))))
        out.write('\n')

    for i in range(numUsers):
        out.write(' '.join(
            ("user_deal_won", str(i), str(datetime.datetime.timestamp(datetime.datetime.now())),
             str(numUsers - i))))
        out.write('\n')
