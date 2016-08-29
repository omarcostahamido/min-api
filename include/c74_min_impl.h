/// @file	
///	@ingroup 	minapi
///	@copyright	Copyright (c) 2016, Cycling '74
///	@license	Usage of this file and its contents is governed by the MIT License

#pragma once

#include "c74_min_api.h"

namespace c74 {
namespace min {

	atoms object_base::try_call(const std::string& name, const atoms& args) {
		auto found_message = m_messages.find(name);
		if (found_message != m_messages.end())
			return (*found_message->second)(args);
		return {};
	}


	// inlets have to be created as a separate step because
	// max creates them from right-to-left
	// note that some objects will not call this function... i.e. dsp objects or other strongly-typed objects.

	void object_base::create_inlets() {
		if (m_inlets.empty())
			return;
		for (auto i = m_inlets.size()-1; i>0; --i)
			m_inlets[i]->m_instance = max::proxy_new(m_maxobj, (long)i, nullptr);
	}

	
	// outlets have to be created as a separate step because
	// max creates them from right-to-left
	void object_base::create_outlets() {
		for (auto outlet = m_outlets.rbegin(); outlet != m_outlets.rend(); ++outlet) {
			if ((*outlet)->type() == "")
				(*outlet)->m_instance = max::outlet_new(m_maxobj, nullptr);
			else
				(*outlet)->m_instance = max::outlet_new(m_maxobj, (*outlet)->type().c_str());
		}
	}


	void timer_tick_callback(timer* a_timer) {
		a_timer->tick();
	}



	// part of the symbol class but must be defined after atom is defined
	symbol::symbol(const atom& value) {
		s = value;
	}

	// part of the symbol class but must be defined after atom is defined
	symbol& symbol::operator = (const atom& value) {
		s = value;
		return *this;
	}

	
	void object_base::process_arguments(const atoms& args) {
		auto argcount = std::min(args.size(), m_arguments.size());

		for (auto i=0; i<argcount; ++i)
			(*m_arguments[i])(args[i]);
	}

	
}} // namespace c74::min