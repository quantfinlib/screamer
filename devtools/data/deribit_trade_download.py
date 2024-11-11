import requests
import pandas as pd
import datetime
from datetime import datetime as dt
from datetime import date
from datetime import timezone
import calendar
import argparse
import os


def datetime_to_timestamp(datetime_obj):
    """Converts a datetime object to a Unix timestamp in milliseconds."""
    return int(datetime_obj.replace(tzinfo=timezone.utc).timestamp() * 1000)


def timestamp_to_datetime(timestamp):
    """Converts a Unix timestamp in milliseconds to a datetime object."""
    return dt.utcfromtimestamp(timestamp / 1000)


def derivative_data(currency: str, kind: str, start_datetime: dt, end_datetime: dt, count: int = 10000) -> pd.DataFrame:
    """Returns derivative trade data for a specified currency and time range.

    Args:
        currency (str): The currency symbol, e.g. 'BTC'.
        kind (str): The type of derivative, either 'option' or 'future'.
        start_date (date): The start date of the time range (inclusive).
        end_date (date): The end date of the time range (inclusive).
        count (int, optional): The maximum number of trades to retrieve per request. Defaults to 10000.

    Returns:
        pandas.DataFrame: A dataframe of derivative trade data for the specified currency and time range.
    """
    start_datetime = start_datetime.replace(tzinfo=timezone.utc)
    end_datetime = end_datetime.replace(tzinfo=timezone.utc)

    # Validate input arguments
    assert isinstance(currency, str), "currency must be a string"
    assert isinstance(start_datetime, dt), "start_date must be a date object"
    assert isinstance(end_datetime, dt), "end_datetime must be a date object"
    assert start_datetime <= end_datetime, "start_datetime must be before or equal to end_datetime"

    derivative_list = []
    params = {
        "currency": currency,
        "kind": kind,
        "include_old": True,
        "start_timestamp": datetime_to_timestamp(start_datetime),
        "end_timestamp": datetime_to_timestamp(end_datetime),
        "sorting": "asc"
    }

    print(params)

    if count:
        params["count"] = count

    url = 'https://history.deribit.com/api/v2/public/get_last_trades_by_currency_and_time'

    # Use a session object to make requests to the API endpoint in a loop, paging through results until all data has been retrieved
    with requests.Session() as session:
        while True:
            response = session.get(url, params=params)
            response_data = response.json()

            print(
                params["start_timestamp"],
                timestamp_to_datetime(params["start_timestamp"]).strftime("%Y-%m-%d %H:%M:%S"),
                len(response_data["result"]["trades"])
            )

            if len(response_data["result"]["trades"]) == 0:
                break
            derivative_list.extend(response_data["result"]["trades"])
            params["start_timestamp"] = response_data["result"]["trades"][-1]["timestamp"] + 1
            if params["start_timestamp"] >= datetime_to_timestamp(end_datetime):
                break

    # Create a pandas dataframe from the derivative trade data
    derivative_data = pd.DataFrame(derivative_list)
    if len(derivative_data) == 0:
        return derivative_data
    derivative_data["date_time"] = pd.to_datetime(derivative_data["timestamp"], unit='ms', utc=True)
    return derivative_data


def main():
    parser = argparse.ArgumentParser(description='Download Deribit trade data for a given currency and date range.')
    parser.add_argument(
        '--start-timestamp', 
        type=str, 
        required=True, 
        help='Start timestamp in ISO 8601 format (e.g., "2020-12-31T12:34:56" or "20201231T123456")'
    )
    parser.add_argument(
        '--end-timestamp', 
        type=str, 
        required=True, 
        help='End timestamp in ISO 8601 format (e.g., "2020-12-31T12:34:56" or "20201231T123456")'
    )
    parser.add_argument(
        '--currency', 
        type=str, 
        default='BTC', 
        help='Currency code (default: BTC)'
    )
    args = parser.parse_args()

    try:
        start_datetime = datetime.datetime.fromisoformat(args.start_timestamp)
        end_datetime = datetime.datetime.fromisoformat(args.end_timestamp)
    except ValueError as e:
        parser.error(f"Invalid timestamp format: {e}")

    if start_datetime >= end_datetime:
        parser.error("Start timestamp must be earlier than end timestamp.")

    currency = args.currency.upper()

    print(f"Downloading data for {args.currency} from {start_datetime} to {end_datetime}")

    df = derivative_data(
        currency=currency,
        kind="future",
        start_datetime=start_datetime,
        end_datetime=end_datetime,
        count=10000
    )

    # Add volumne column
    df["volume"] = df.apply(lambda row: row["amount"] if row["direction"] == "buy" else -row["amount"], axis=1)

    # we only want the BTC-PERPETUAL rows
    df = df[df['instrument_name'] == currency.upper() + '-PERPETUAL']

    df_small = df[['timestamp', 'volume', 'price']]

    # adjust:
    # timestamp, volume, price
    # timestamp, direction  amount

    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Format the datetime values to ISO 8601 format suitable for filenames
    start_iso = start_datetime.strftime('%Y%m%dT%H%M%S')
    end_iso = end_datetime.strftime('%Y%m%dT%H%M%S')


    output_filename = f'deribit.trades.{currency.lower()}-perpetual.{start_iso}.{end_iso}.csv'
    output_filepath = os.path.join(script_dir, output_filename)
    df_small.to_csv(output_filepath, index=False)
    print(f"Data saved to {output_filepath}")

if __name__ == '__main__':
    main()