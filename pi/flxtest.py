'''Flexx test file'''
from flexx import flx
import csv

with open("sampledata.csv") as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    line_count = 0
    data = []
    for row in csv_reader:
        if line_count == 0:
            print(f'Column names are {",".join(row)}')
            line_count+=1
        else:
            print("Appeding")
            data.append(row)
            line_count+=1


months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul',
            'Aug', 'Sep', 'Oct', 'Nov', 'Dec', 'total']


dates = []
rownumber = 0
for row in data:
    date = data[rownumber][0]
    if(date in dates) == False:
        dates.append(date)
    rownumber+=1



def date_data(date):
    '''This function parses the data list to obtain all the relevent data for a specified date. This includes both temp and times, so if the amount of recordings is not the same between any given days, it will still work.'''
    times = []
    temps = []
    rownumber=0
    for row in data:
        if(date == data[rownumber][0]):
            times.append(data[rownumber][1])
            temps.append(data[rownumber][2])

        rownumber += 1
    return times, temps




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
                self.plot = flx.PlotWidget(flex=1,
                                           xdata=date_data(self.date.selected_key)[0], yrange=(0, 50),
                                           title='Test',
                                           xlabel='Time', ylabel=u'temperature (°C)')
                self.plot2 = flx.PlotWidget()
            flx.Widget(flex=1)

    @flx.reaction
    def _update_plot(self):
        print("Selected Date: " + self.date.selected_key)
        self.plot.set_data(date_data(self.date.selected_key)[0], date_data(self.date.selected_key)[1])
