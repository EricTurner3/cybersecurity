import datetime

'''
HTB - Intelligence
30 Aug 2021

After enumeration, I discovered that there were files in the /documents directory on the web server that followed a particular format
This script generates all of the dates back to Jan 1 2020 as a wordlist to be used with gobuster

$ gobuster dir -u http://10.129.122.188/documents -w /home/kali/htb/dates.txt -e > docs.txt
'''

# 2020-01-01 is 608 days from 2021-08-30
numdays = 973

base = datetime.datetime.today()
date_list = [base - datetime.timedelta(days=x) for x in range(numdays)]
print('Generating {} file names'.format(str(numdays)))
file_names = [dt.date().strftime("%Y-%m-%d-upload.pdf") for i, dt in enumerate(date_list)]

#print (file_names)
f = open('dates.txt', 'w')
for dt in file_names:
    f.write(str(dt) + '\n')
print('Outputted to file ./dates.txt')
f.close()