/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/**
 * @author Christian <c@ethdev.com>
 * @date 2015
 * Evaluator for types of constant expressions.
 */

#pragma once

#include <libsolidity/ast/ASTVisitor.h>

#include <utility>

namespace solidity::langutil
{
class ErrorReporter;
}

namespace solidity::frontend
{

class TypeChecker;

/**
 * Small drop-in replacement for TypeChecker to evaluate simple expressions of integer constants.
 */
class ConstantEvaluator: private ASTConstVisitor
{
public:
	using EvaluationMap = std::map<ASTNode const*, TypePointer>;

	ConstantEvaluator(langutil::ErrorReporter& _errorReporter, EvaluationMap& _evaluations):
		m_errorReporter{ _errorReporter },
		m_evaluations{ _evaluations },
		m_depth{ 0 }
	{
	}

	static TypePointer evaluate(
		langutil::ErrorReporter& _errorReporter,
		Expression const& _expr
	);

	TypePointer evaluate(Expression const& _expr);

private:
	void endVisit(BinaryOperation const& _operation) override;
	void endVisit(UnaryOperation const& _operation) override;
	void endVisit(Literal const& _literal) override;
	void endVisit(Identifier const& _identifier) override;
	void endVisit(TupleExpression const& _tuple) override;

	void setType(ASTNode const& _node, TypePointer const& _value);

	TypePointer type(ASTNode const& _node);

	bool evaluated(ASTNode const& _node) const noexcept;

	langutil::ErrorReporter& m_errorReporter;
	EvaluationMap& m_evaluations;
	/// Current recursion depth.
	size_t m_depth = 0;
};

}
