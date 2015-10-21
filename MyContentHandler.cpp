//
// Created by 顾伟刚 on 15/10/18.
//

#include "MyContentHandler.h"

binary_log::Binary_log_event* MyContentHandler::process_event(binary_log::Table_map_event *tm)
{
    m_table_id = tm->get_table_id();
    m_table_index.insert(Int2event_map::value_type(tm->get_table_id(), tm));
    /* Consume this event so that it won't be deallocated */
    return 0;
}

binary_log::Binary_log_event* MyContentHandler::process_event(binary_log::Rows_event *event)
{
    rows_val.clear();
    Int2event_map::iterator ti_it= m_table_index.find(event->get_table_id());
    if (ti_it != m_table_index.end())
    {
        binary_log::Row_event_set rows(event, ti_it->second);
        binary_log::Row_event_set::iterator row_it = rows.begin();
        do
        {
            row_fields_val.clear();
            binary_log::Row_of_fields fields = *row_it;
            binary_log::Row_of_fields::iterator field_it = fields.begin();
            binary_log::Converter converter;
            if (field_it != fields.end())
                do
                {
                    std::string str;
                    converter.to(str, *field_it);
                    Row_Fields_pair field;
                    field.first = (*field_it).type();
                    field.second = str;
                    row_fields_val.push_back(field);
                } while (++field_it != fields.end());

            rows_val.push_back(row_fields_val);

        } while (++row_it != rows.end());

    } //end if
    return event;
}