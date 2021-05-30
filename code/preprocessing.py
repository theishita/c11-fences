from constants import *

def preprocessing(order):
  reads = []
  writes = []

  for i in range(len(order)):
    if order[i][TYPE] == WRITE or order[i][TYPE] == RMW:
      writes.append(order[i-1])
      writes.append(order[i])
      writes.append(order[i+1])
    if order[i][TYPE] == READ or order[i][TYPE] == RMW:
      reads.append(order[i-1])
      reads.append(order[i])
      reads.append(order[i+1])
  
  return reads, writes