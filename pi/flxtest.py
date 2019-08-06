'''Flexx test file'''
from flexx import flx
import csv

months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul',
            'Aug', 'Sep', 'Oct', 'Nov', 'Dec', 'total']


logfile_names = ["electrolyser.csv","inverter.csv"]
electrolyserData = []
inverterData = []
data = [electrolyserData,inverterData]

for i in range(0,2):
    with open(logfile_names[i]) as csv_file:
        csv_reader = csv.reader(csv_file,delimiter=',')
        line_count = 0
        
        for row in csv_reader:
            if line_count == 0:
                print(f'Column names are {",".join(row)}')
                line_count+=1
            else:
                print("Appeding")
                data[i].append(row)
                line_count+=1


dates = []
rownumber = 0
for i in range(0,2):
    rownumber = 0
    for row in data[i]:
        date = data[i][rownumber][0]
        if(date in dates) == False:
            dates.append(date)
        rownumber+=1


def date_data(date):
    '''This function parses the data list to obtain all the relevent data for a specified date. This includes both temp and times, so if the amount of recordings is not the same between any given days, it will still work.'''
    

    timeselectrolyser = []
    flowselectrolyser = []
    H2Totalselectrolyser = []
    voltageselectrolyser = []
    pressureselectrolyser = []
    tempselectrolyser = []

    timesinverter = []
    currentPowerInverter = []
    energyTodayInverter = []
    totalEnergyInverter = []


    times = [timeselectrolyser,timesinverter]

 
    
    rownumber=0

    for i in range(0,2):
        rownumber = 0
        workingdata = data[i]
        for row in workingdata:

            if i == 0:
                if(date == workingdata[rownumber][0]):
                    times[i].append(workingdata[rownumber][1])
                    flowselectrolyser.append(workingdata[rownumber][2])
                    H2Totalselectrolyser.append(workingdata[rownumber][3])
                    voltageselectrolyser.append(workingdata[rownumber][4])
                    pressureselectrolyser.append(workingdata[rownumber][5])
                    tempselectrolyser.append(workingdata[rownumber][6])
            if i == 1:
                if(date == workingdata[rownumber][0]):
                    times[i].append(workingdata[rownumber][1])
                    currentPowerInverter.append(workingdata[rownumber][2])
                    energyTodayInverter.append(workingdata[rownumber][3])
                    totalEnergyInverter.append(workingdata[rownumber][4])

            rownumber += 1
    electrolyserParameters = [flowselectrolyser,H2Totalselectrolyser,voltageselectrolyser,pressureselectrolyser,tempselectrolyser]
    inverterParameters = [currentPowerInverter,energyTodayInverter,totalEnergyInverter]
    return times, electrolyserParameters, inverterParameters

[testtimes,testelec,testinv] = date_data('2019_06_25')
print("STOP")

# This class is responsible for plotting the graph of a temperature readout.
class TempGraph(flx.Widget):
    def init(self):
        with flx.HFix():
            flx.Widget(flex=1)
            with flx.VBox(flex=0, minsize=200):
                with flx.GroupWidget(title='Plot options'):
                    flx.Label(text='Date')
                    self.date = flx.ComboBox(options=dates, selected_index=len(dates)-1, style='width: 100%')
                flx.Widget(flex=3)
            with flx.VBox(flex=4):
                self.electempplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 50),
                                           title='Electrolyser ',
                                           xlabel='Time', ylabel=u'temperature (°C)')
                self.elecflowplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 50),
                                           title='',
                                           xlabel='Time', ylabel=u'flow')
                self.elecH2Totalplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 2000),
                                           title='',
                                           xlabel='Time', ylabel=u'H2Total')
                self.elecvoltageplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 20),
                                           title='',
                                           xlabel='Time', ylabel=u'Voltage')
                self.elecpressureplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 600),
                                           title='',
                                           xlabel='Time', ylabel=u'Pressure')




                self.invpowerplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 50),
                                           title='Inverter',
                                           xlabel='Time', ylabel=u'Power')
                self.invenergyplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 1200),
                                           title='Inverter',
                                           xlabel='Time', ylabel=u'Power')
                self.invtotalenergyplot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 10000),
                                           title='Inverter',
                                           xlabel='Time', ylabel=u'Power')
            flx.Widget(flex=1)
    
    @flx.reaction
    def _update_plot(self):
        print("Selected Date: " + self.date.selected_key)
        
        self.elecflowplot.set_data(date_data(self.date.selected_key)[0][0], date_data(self.date.selected_key)[1][0])
        self.elecH2Totalplot.set_data(date_data(self.date.selected_key)[0][0], date_data(self.date.selected_key)[1][1])
        self.elecvoltageplot.set_data(date_data(self.date.selected_key)[0][0], date_data(self.date.selected_key)[1][2])
        self.elecpressureplot.set_data(date_data(self.date.selected_key)[0][0], date_data(self.date.selected_key)[1][3])


        self.invpowerplot.set_data(date_data(self.date.selected_key)[0][1], date_data(self.date.selected_key)[2][0])
        self.invenergyplot.set_data(date_data(self.date.selected_key)[0][1], date_data(self.date.selected_key)[2][1])
        self.invtotalenergyplot.set_data(date_data(self.date.selected_key)[0][1], date_data(self.date.selected_key)[2][2])
