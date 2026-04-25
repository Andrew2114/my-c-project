import csv

with open("airport-codes.csv", encoding="utf-8") as fin, \
     open("airports.txt", "w", encoding="utf-8") as fout:
    reader = csv.DictReader(fin)
    for row in reader:
        code = row["iata_code"].strip()
        name = row["name"].strip()
        if code:
            fout.write(f"{code}:{name}\n")
