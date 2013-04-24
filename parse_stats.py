#!/bin/python
##
## Stat gathering
##

import sys
import os
import re
import string
import getopt
import array
import copy
import numpy
import math

def usage():
  print "\
Usage: parse_stats.py -[hf:]\n\
  -h --help           print this help\n\
  -f --filename       specify libstdcxx-profile.raw filename\n\
  "

stat_by_field = dict(init=0, 
    count=1, 
    cost=2, 
    resize=3, 
    min=4, 
    max=5,
    total=6, 
    item_min=7, 
    item_max=8, 
    item_total=9,
    insert=10, 
    erase=11,
    find=12,
    lower_bound=13,
    upper_bound=14,
    equal_range=15,
    push_back=16,
    insert_time=17,
    erase_time=18,
    find_time=19,
    lower_bound_time=20,
    upper_bound_time=21,
    equal_range_time=22,
    push_back_time=23
    )

columns = dict(
    summary=0,
    insert=1,
    erase=2,
    find=3,
    lower_bound=4,
    upper_bound=5,
    equal_range=6,
    push_back=7
    )

##
# get_statistics
# Puts the stats into a 2d list. Each row contains one entry for the stat
# First column contains the stat name, backtrace, and summarized statistics.
# Remaining columns contain details.
##
def get_statistics(filename, statname):
  result = [];
  file = open(filename, 'r')
  lines = file.readlines()
  for x in xrange(len(lines)):
    line = lines[x]
    if re.match(statname+".*", line):
      cur = []
      cur.append(string.strip(line).split('|'))
      for i in xrange(len(columns)):
        if i == 0:
          continue
        line = lines[x+i]
        cur.append(string.strip(line))
      result.append(cur)
  return result

def get_summary_stat(all_results, statname):
  r = [];
  for x in all_results:
    r.append(int(x[columns["summary"]][2].split(' ')[stat_by_field[statname]]))
  return r

def get_detail_stat(all_results, statname):
  r = [];
  if statname == "summary":
    return r
  for x in all_results:
    v = []
    row = x[columns[statname]].split(' ')
    v.append([row[0]]) ## name
    time = []
    latency = []
    for i in xrange(len(row)):
      if i == 0:
        continue
      if i % 2 == 0:
        latency.append(int(row[i]))
      else:
        time.append(int(row[i]))
    for i in xrange(len(latency)):
      if latency[i] > 100000000: # arbitrary number for pruning.
        #print "pruning " + str(latency[i])
        latency[i] = numpy.NaN # is this ok?
    v.append(time)
    v.append(latency)
    r.append(v)
  return r

## for whatever reason, numpy does not include a nanmean
def nanmean(arr):
  a = numpy.array(arr)
  return numpy.nansum(a)/float(len(arr) - sum(numpy.isnan(a)))

## or a nanstd
def nanstd(arr):
  m = nanmean(arr)
  std = math.sqrt(nanmean([numpy.abs(x - m)**2 for x in arr]))
  return std

def compute_statistics(raw_stats):

  summary_stats = [[] for x in stat_by_field.keys()]
  for k, v in stat_by_field.iteritems():
    summary_stats[v] = get_summary_stat(raw_stats, k)
  item_total_list = summary_stats[stat_by_field["item_total"]]
  print "item_total",
  #print " ".join(map(str, item_total_list))
  item_total_array = numpy.array(item_total_list)
  print len(item_total_list),
  print numpy.mean(item_total_list),
  print numpy.std(item_total_array),
  print numpy.amin(item_total_array),
  print numpy.amax(item_total_array),
  print numpy.nansum(item_total_array)

  detail_stats = [[] for x in columns.keys()]
  for k, v in columns.iteritems():
    detail_stats[v] = get_detail_stat(raw_stats, k)
  #print detail_stats[columns["insert"]]
  #print detail_stats[columns["summary"]]

  ## compute some statistics
  op_count = [[] for x in columns.keys()]
  mean_latency = [[] for x in columns.keys()]
  std_latency = [[] for x in columns.keys()]
  sum_latency = [[] for x in columns.keys()]
  min_latency = [[] for x in columns.keys()]
  max_latency = [[] for x in columns.keys()]
  mean_ia = [[] for x in columns.keys()]
  std_ia = [[] for x in columns.keys()]
  sum_ia = [[] for x in columns.keys()]
  min_ia = [[] for x in columns.keys()]
  max_ia = [[] for x in columns.keys()]
  for k, v in columns.iteritems():
    if v == 0:
      continue
    #mean_latency[v] = [numpy.mean(numpy.array(x[2])) for x in detail_stats[v]]
    #std_latency[v] = [numpy.std(numpy.array(x[2])) for x in detail_stats[v]]
    #sum_latency[v] = [sum(x[2]) for x in detail_stats[v]]
    # deal with stats that need massaging for empty elements
    for x in detail_stats[v]:
      time_list = x[1]
      latency_list = x[2]
      ia_list = [time_list[i+1]-time_list[i] for i in xrange(len(time_list)-1)]
      time_array = []
      latency_array = []
      ia_array = []
      if not latency_list:
        time_array = numpy.array([numpy.NaN])
        latency_array = numpy.array([numpy.NaN])
      else:
        time_array = numpy.array(time_list)
        latency_array = numpy.array(latency_list)
      if not ia_list:
        ia_array = numpy.array([numpy.NaN])
      else:
        ia_array = numpy.array(ia_list)
      op_count[v].append(len(latency_list))
      mean_latency[v].append(numpy.mean(latency_list))
      std_latency[v].append(numpy.std(latency_array))
      sum_latency[v].append(numpy.nansum(latency_array))
      min_latency[v].append(numpy.amin(latency_array))
      max_latency[v].append(numpy.amax(latency_array))
      mean_ia[v].append(numpy.mean(ia_array))
      std_ia[v].append(numpy.std(ia_array))
      sum_ia[v].append(numpy.sum(ia_array))
      min_ia[v].append(numpy.amin(ia_array))
      max_ia[v].append(numpy.amax(ia_array))

  sum_time = 0
  for k,v in columns.iteritems():
    if v == 0:
      continue
    if mean_latency[v] and numpy.sum(numpy.array(mean_latency[v]) > 0):
      print k,
      print len(min_latency[v]) - sum(numpy.isnan(min_latency[v])),
      oca = numpy.array(op_count[v])
      print numpy.sum(oca), numpy.mean(oca), numpy.std(oca),# numpy.amax(oca),
      #a = numpy.argmax(oca)
      #print sum_latency[v][a], item_total_list[a],
      a = numpy.nanargmax(numpy.array(sum_latency[v]))
      print op_count[v][a],
      print sum_latency[v][a], item_total_list[a],
      print nanmean(mean_latency[v]),
      print nanmean(std_latency[v]),
      print int(numpy.nanmin(numpy.array(min_latency[v]))),
      print int(numpy.nanmax(numpy.array(max_latency[v]))),
      print int(numpy.nansum(numpy.array(sum_latency[v])))
      sum_time = sum_time + int(numpy.nansum(numpy.array(sum_latency[v])))
  print "sum_time", sum_time
  return sum_time


##
# Script entry point
##
def main():
  # default args
  filename="libstdcxx-profile.raw"
  try:
    opts, args = getopt.getopt(sys.argv[1:], "hf:",
        ["help","filename="])
  except getopt.GetoptError, err:
    print str(err)
    usage()
    sys.exit(2)
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
      sys.exit()
    elif opt in ("-f", "--filename"):
      filename = arg
    else:
      assert False, "unhandled option"

  total_time = 0

  print "map-statistics"
  map_stats = get_statistics(filename,"map-statistics")
  total_time = total_time + compute_statistics(map_stats)
  
#  print "vector-statistics"
#  vector_stats = get_statistics(filename,"vector-statistics")
#  total_time = total_time + compute_statistics(vector_stats)

  print "total_time", total_time

if __name__ == "__main__":
  main()
