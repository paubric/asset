import dash
from dash.dependencies import Input, Output
import dash_core_components as dcc
import dash_html_components as html
from datetime import datetime as dt
import numpy as np
import bridge_controller as bc

app = dash.Dash()

visible_samples = 30
temperature_data = []
pressure_data = []

app.layout = html.Div([
    html.H1('PARROT'),
    html.H2('Particle Accelerator Tool'),
    dcc.Interval(id='my-interval'),
    dcc.RadioItems(id='set-time',
        value=1000,
        options=[
            {'label': 'Every second', 'value': 1000},
            {'label': 'Every 5 seconds', 'value': 5000},
            {'label': 'Off', 'value': 60*60*1000} # or just every hour
        ]),
    dcc.Graph(id='graph')])

@app.callback(
    dash.dependencies.Output('my-interval', 'interval'),
    [dash.dependencies.Input('set-time', 'value')])
def update_interval(value):
    return value

@app.callback(Output('graph', 'figure'),  events=[dash.dependencies.Event('my-interval', 'interval')])
def update_graph():
    response = bc.analog_read_all('rtua', 0)
    print(response)
    temperature_data.append(response['value'])

    return {
        'data': [{
            'x': list(range(len(temperature_data[-visible_samples:]))),
            'y': temperature_data[-visible_samples:],
            'name':'Temperature'
        }]
    }

if __name__ == '__main__':
    app.run_server()
