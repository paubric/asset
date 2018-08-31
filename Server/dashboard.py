import flask
import dash
from dash.dependencies import Output, Event
import dash_core_components as dcc
import dash_html_components as html
import plotly
import random
import plotly.graph_objs as go
from collections import deque
import bridge_controller as bc

X_temp = deque(maxlen=20)
X_temp.append(1)
Y_temp = deque(maxlen=20)

X_pres = deque(maxlen=20)
X_pres.append(1)
Y_pres = deque(maxlen=20)

X_humi = deque(maxlen=20)
X_humi.append(1)
Y_humi = deque(maxlen=20)

X_conc = deque(maxlen=20)
X_conc.append(1)
Y_conc = deque(maxlen=20)

app = dash.Dash(__name__, processes=4)

app.layout = html.Div(
    [
        dcc.Graph(id='live-graph'),
        dcc.Graph(id='live-graph2'),
        dcc.Graph(id='live-graph3'),
        dcc.Graph(id='live-graph4'),
        dcc.Interval(
            id='graph-update',
            interval=2*1000
        ),
        html.Img(id="img",src="http://192.168.162.254:10000/cgi-bin/video.cgi?msubmenu=mjpg",width="1024",height="768")
    ]
)

@app.callback(Output('live-graph', 'figure'),
              events=[Event('graph-update', 'interval')])
def update_graph_scatter():
    response = bc.analog_read_pin('rtua', 0)
    X_temp.append(X_temp[-1]+1)
    Y_temp.append(response)
    data = plotly.graph_objs.Scatter(
            x=list(X_temp),
            y=list(Y_temp),
            name='Scatter',
            mode= 'lines+markers'
            )

    return {'data': [data]}

@app.callback(Output('live-graph2', 'figure'),
              events=[Event('graph-update', 'interval')])
def update_graph_scatter():
    response = bc.analog_read_pin('rtua', 1)
    X_pres.append(X_pres[-1]+1)
    Y_pres.append(response)
    data = plotly.graph_objs.Scatter(
            x=list(X_pres),
            y=list(Y_pres),
            name='Scatter',
            mode= 'lines+markers'
            )

    return {'data': [data]}

@app.callback(Output('live-graph3', 'figure'),
              events=[Event('graph-update', 'interval')])
def update_graph_scatter():
    response = bc.analog_read_pin('rtub', 0)
    X_humi.append(X_humi[-1]+1)
    Y_humi.append(response)
    data = plotly.graph_objs.Scatter(
            x=list(X_humi),
            y=list(Y_humi),
            name='Scatter',
            mode= 'lines+markers'
            )

    return {'data': [data]}

@app.callback(Output('live-graph4', 'figure'),
              events=[Event('graph-update', 'interval')])
def update_graph_scatter():
    response = bc.analog_read_pin('rtub', 1)
    X_conc.append(X_conc[-1]+1)
    Y_conc.append(response)
    data = plotly.graph_objs.Scatter(
            x=list(X_conc),
            y=list(Y_conc),
            name='Scatter',
            mode= 'lines+markers'
            )

    return {'data': [data]}


if __name__ == '__main__':
    app.run_server(debug=True)