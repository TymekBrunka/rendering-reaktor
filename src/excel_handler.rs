use rfd::FileDialog;
use calamine::{Reader, open_workbook, Xlsx, Data};

use crate::types::Row;

struct ColumnMapping {
	KNKD: Box<[Option<usize>; 41]>
}

impl Default for ColumnMapping {
	fn default() -> Self {
		Self {
			KNKD: Box::new([None; 41])
		}
	}
}

pub fn click_action() -> Result<Vec<Row>, ()> {
	let mut colstart = 0;
	let mut colend = 0;
	// let mut rowstart = 0;
	let mut colmap: ColumnMapping = Default::default();
	let mut found_header_row = false;

	let mut rows: Vec<Row> = Vec::new();

	let file = FileDialog::new()
        .add_filter("excel", &["xlsx", "xls"])
        .set_directory("/")
        .pick_file();

    if file.is_some() {
        let file = file.unwrap();
        let filepath = file.as_path().to_str().unwrap();
        let mut workbook: Xlsx<_> = open_workbook(filepath).expect("Nie można otworzyć pliku.");
        if let Ok(r) = workbook.worksheet_range("Arkusz1") {

        	//finding header row
        	// let mut it: Data;
        	let mut iter = r.rows().enumerate().into_iter();
            while let Some((_, row)) = iter.next() {
                if row.len() > 0 {
                	// println!("row={:?}, row[0]={:?}", row, row[0]);
                	//checking if it has DataCzas column
                	for (j, column) in row.iter().enumerate() {
                		match column {
                			Data::String(name) => {
                				match name.as_str() {
                					"DataCzas" => {
		                				// rowstart = i;
		                				colstart = j;
		                				found_header_row = true;
		                			}
		                			"wwRetrievalMode" => {colend = j;}
		                			string => {
		                				if string.ends_with("KNKD") {
		                					let col: usize = string[0..string.len() - 5].parse().map_err(|_| ())?;

		                					colmap.KNKD[col - 1] = Some(j);
		                				}
		                			}
		                		}
                			},
                			_ => {}
                		}
                	}
                }

                if found_header_row { break; }
            }

            let mut row_to_be_added: Row = Default::default();

            while let Some((_, row)) = iter.next() {
            	//
            	// filling row data
            	//

            	if let Data::DateTime(dataczas) = row[colstart] {
            		if row.len() < colend { continue; }
            		
            		row_to_be_added = Row {
            			dataczas: row_to_be_added.dataczas,
            			..Default::default()
            		};
            		row_to_be_added.dataczas = dataczas.as_datetime().unwrap().to_string();

            		rows.push(row_to_be_added.clone());

            		// println!("{:?}", row);
            	}

            	for i in 0..20 {
            		if let Some(j) = colmap.KNKD[i] {
	            		if let Data::Float(f) = row[j] {
	            			let len = rows.len() - 1;
	            			rows[len].KNKD[i] = f;
	            			rows[len].KNKDs[i] = format!("{:.2}mg/l", f);
	            		}
	            	}
	            }
            	
            }
        }
    }

    Ok(rows)
}