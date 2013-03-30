#!/bin/bash
# The only parameter required is the instance name

function createReportFile{
	touch $1
	echo "Instance	Infeasible	PRDP	CpuTime" >> $1
}

createReportFile transpose.first
createReportFile exchange.first
createReportFile insert.first
createReportFile transpose.best
createReportFile exchange.best
createReportFile insert.best
